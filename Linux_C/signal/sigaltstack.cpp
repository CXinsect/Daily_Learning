//#define _GNU_SOURCE
#define _XOPEN_SOURCE  600
#include <iostream>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
static void sigSethandler(int sig) {
    int x;
    std::cout<<"capture signal: "<<sig<<"discribe: "
                <<strsignal(sig)<<std::endl;
    printf("Top of handler stack near: %10p\n",(void *)&x);
    fflush(NULL);
    _exit(EXIT_FAILURE);
}
static void overFlowstack(int callNum) {
    char a[100000];
    printf("Call %4d -Top of handler stack near: %10p\n",callNum,&a[0]);
    overFlowstack(callNum + 1);
}
int main (int argc,char *argv[]) {
    stack_t sigstack;
    struct sigaction sa;
    int j;
    printf("Top of standard stack is near %10p\n",(void *)&j);
    sigstack.ss_sp = malloc(SIGSTKSZ);
    if(sigstack.ss_sp == NULL) {
        std::cout<<"sigstack err"<<std::endl;
        exit(0);
    }
    sigstack.ss_size = SIGSTKSZ;
    sigstack.ss_flags = 0;
    if(sigaltstack(&sigstack,NULL) == -1) {
        std::cout<<"sigstack err"<<std::endl;
        exit(0);
    }
    printf("alternate stack is at %10p - %10p",sigstack.ss_sp,(char*)sbrk(0) -1);
    sa.sa_handler = sigSethandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;
    if(sigaction(SIGSEGV,&sa,NULL) == -1) {
        std::cout<<"sigstack err"<<std::endl;
        exit(0);
    }
    overFlowstack(1);
    return 0;
}