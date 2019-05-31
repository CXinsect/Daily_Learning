#include "sig_function.cpp"
#include <iostream>
#include <signal.h>
#include <time.h>
#include <string.h>
static volatile sig_atomic_t sigQuit = 0;
static void handler (int sig) {
    std::cout<<"Caught signal: "<<sig<<"discribe: "<<strsignal(sig)<<std::endl;
    if(sig == SIGQUIT) {
        sigQuit = 1;
    }
}
int main (int argc,char *argv[]) {
    int loopNum;
    time_t startTime;
    sigset_t preMask,oldMask;
    struct sigaction sa;
    printSigMask(stdout,"Initial signal Mask\n");
    sigemptyset(&oldMask);
    sigaddset(&oldMask,SIGINT);
    sigaddset(&oldMask,SIGQUIT);
    //阻塞信号
    if(sigprocmask(SIG_BLOCK,&oldMask,&preMask) == -1) {
        std::cout<<"sigprocmask err"<<std::endl;
        exit(EXIT_FAILURE);
    }
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if(sigaction(SIGQUIT,&sa,NULL) == -1) {
        std::cout<<"sigaction err"<<std::endl;
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGINT,&sa,NULL) == -1) {
        std::cout<<"sigaction err"<<std::endl;
        exit(EXIT_FAILURE);
    }
    for(loopNum = 1;!sigQuit;loopNum++) {
        std::cout<<"loopNum: "<<loopNum<<std::endl;
        //printSigMask(stdout,"Initial signal Mask11\n");
        
        //模拟代码执行
        for(startTime = time(NULL);time(NULL) < startTime + 3;) 
            continue;

        //打印信号掩码
       // printPendinfSigs(stdout,"before sigsuspend() - pending mask\n");
        //解除对相应信号的阻塞等待信号到来接着恢复阻塞状态
        if(sigsuspend(&preMask) == -1  && errno != EINTR) {
            std::cout<<"sigsuspend err"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    if(sigprocmask(SIG_SETMASK,&preMask,NULL) == -1) {
        std::cout<<"sigprocmask err"<<std::endl;
        exit(EXIT_FAILURE);
    }
    printSigMask(stdout,"exited loop: sig mask\n");
    return 0;
}