#include <iostream>
#include <signal.h>
#include <string>
#include <unistd.h>

int main (void) {
    sigset_t blockset,preMask;
    sigemptyset(&blockset);
    sigaddset(&blockset,SIGINT);
    if(sigprocmask(SIG_BLOCK,&blockset,&preMask) == -1) 
        std::cout<<"sigprocmask err"<<std::endl;
    std::cout<<"Ctrl_c 信号已被阻塞"<<std::endl;    
    sleep(5);
    std::cout<<"解除阻塞"<<std::endl;
    if(sigprocmask(SIG_SETMASK,&preMask,NULL) == -1)
        std::cout<<"sigprocmask err"<<std::endl;
    return 0;
}