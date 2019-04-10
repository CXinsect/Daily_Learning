#include "model.h"

using namespace std;


void printpendingset(sigset_t * sig) {
    for(int i = 1;i < 64;i++) {
        if(sigismember(sig,i))
            putchar('1');
        else
            putchar('0');
    }
    cout<<endl;
}
void handler (int sig) {
    if(sig == SIGINT) {
        cout<<"已经检测，继续监禁"<<endl;
        sleep(3);
    }
    else if(sig == SIGQUIT) {
        sigset_t uset;
        sigemptyset(&uset);
        sigaddset(&uset,SIGINT);
        sigprocmask(SIG_UNBLOCK,&uset,NULL);
        cout<<"刑满释放"<<endl;
    }
    return;
}
int main(void)
{
    sigset_t pset;
    sigset_t bset;
    sigemptyset(&bset);
    sigaddset(&bset,SIGINT);
    if(signal(SIGINT,handler) == SIG_ERR) 
        ERR_EXIT("signal");
    if(signal(SIGQUIT,handler) == SIG_ERR)
        ERR_EXIT("signal");
    sigprocmask(SIG_BLOCK,&bset,NULL);
    while(1) {
        sigpending(&pset);
        printpendingset(&pset);
        sleep(2);
    }
    return 0;
}

