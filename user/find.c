#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path,const char *fileName){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    //参数错误，find的第一个参数必须是目录
    if (st.type != T_DIR) {
        fprintf(2, "usage: find <DIRECTORY> <filename>\n");
        close(fd);
        return;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_DIR && strcmp(de.name,".") != 0 && strcmp(de.name,"..") != 0)
            find(buf,fileName);
        else if(strcmp(de.name,fileName) == 0)
            printf("%s\n",buf);
    }

    close(fd);
}

int main(int argc,char *argv[]){
    if(argc != 3){
        fprintf(2,"Usage: find...\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}