#include "kernel/types.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(2,"Usage: xargs...\n");
        exit(1);
    }
    int pidx = 0,cidx = 0,k;
    char *parameter[32];
    char buf[32];
    char cur_parameter[32];
    char *p = cur_parameter;
    for(int i = 1;i < argc;i++)
        parameter[pidx++] = argv[i];
    //开始从stdin读入参数
    while((k = read(0,buf,sizeof(buf))) > 0){
        for(int i = 0;i < k;i++){
            if(buf[i] == '\n'){
                cur_parameter[cidx++] = 0;
                parameter[pidx++] = p;
                p = cur_parameter;
                cidx = 0,pidx = argc - 1;
                if(fork() == 0)
                    exec(argv[1],parameter);
                wait(0);
            }else if(buf[i] == ' '){
                cur_parameter[cidx++] = 0;
                parameter[pidx++] = p;
                p = &cur_parameter[cidx];
            }else{
                cur_parameter[cidx++] = buf[i];
            }
        }
    }
    exit(0);
}