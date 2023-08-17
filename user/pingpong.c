#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc,char *argv[])
{
    if(argc != 1){
        fprintf(2,"Usage: pingpong...\n");
        exit(1);
    }
    int ftos[2],stof[2];
    pipe(ftos);
    pipe(stof);
    char sendbuf[1] = {'X'};
    if(fork() == 0){//子进程
        close(ftos[1]);
        close(stof[0]);
        char recv[1];
        read(ftos[0],recv,sizeof(recv));
        printf("%d: received ping\n",getpid());
        write(stof[1],sendbuf,sizeof(sendbuf));
        exit(0);
    }else{//父进程
        close(ftos[0]);
        close(stof[1]);
        char recv[1];
        write(ftos[1],sendbuf,sizeof(sendbuf));
        read(stof[0],recv,sizeof(recv));
        printf("%d: received pong\n",getpid());
        wait(0);//等待子进程返回
        exit(0);
    }
}