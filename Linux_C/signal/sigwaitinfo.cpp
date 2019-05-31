#include <iostream>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
int main (int argc,char *argv[]) {
    int sig;
    siginfo_t si;
    sigset_t allSigs;
    std::cout<<"Pid: "<<getpid()<<std::endl;
    sigfillset(&allSigs);
    if(sigprocmask(SIG_SETMASK,&allSigs,NULL) == -1) {
        std::cout<<"sigprocmask err"<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<"sleep begin"<<std::endl;
    sleep(10);
    std::cout<<"sleep end"<<std::endl;
    for(;;) {
        sig = sigwaitinfo(&allSigs,&si);
        if(sig == -1) {
            std::cout<<"sigwaitinfo err"<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(sig == SIGQUIT || sig == SIGINT)
            exit(EXIT_SUCCESS);
        printf("Caught Sig:%d ,si_code:%s \n",sig,(si.si_code == SI_USER) ? "si_user" :
                                                (si.si_code == SI_QUEUE) ? "SI_QUEUE" : "OTHER");
    }
    return 0;       
}