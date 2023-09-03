#include "kernel/types.h"
#include "user/user.h"

#define WR 1
#define RD 0

const int INT_LEN = 4;

int get_first_data(int lpipe[2],int *first){
    if(read(lpipe[RD],first,INT_LEN) != 0){
        printf("prime %d\n",*first);
        return 0;
    }else{
        return -1;
    }
}

void transmit_data(int lpipe[2],int rpipe[2],int first){
    int tmp;
    while(read(lpipe[RD],&tmp,INT_LEN) != 0){
        if(tmp % first != 0)
            write(rpipe[WR],&tmp,INT_LEN);
    }
    close(lpipe[RD]);
}

void primes(int lpipe[2]){
    close(lpipe[WR]);
    int first;
    if(get_first_data(lpipe,&first) == 0){
        int p[2];
        pipe(p);
        transmit_data(lpipe,p,first);
        if(fork() == 0){
            primes(p);
        }else{
            close(p[WR]);
            close(p[RD]);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc,char *argv[]){
    if(argc != 1){
        fprintf(2,"Usage: primes...\n");
        exit(1);
    }
    int p[2];
    pipe(p);
    for(int i = 2;i <= 35;i++)
        write(p[WR],&i,INT_LEN);
    if(fork() == 0){
        primes(p);
    }else{
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    exit(0);
}