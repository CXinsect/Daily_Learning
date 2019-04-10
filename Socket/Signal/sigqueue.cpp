#include "model.h"

static int count = 0;
void handler (int sig, siginfo_t *s, void * p) {
    sigval_t t;
    cout<<"count: "<<count++<<endl;
    cout<<"sig: "<<sig<<endl;
    t = s->si_value;
    cout<<"valure: "<<t.sival_int<<endl;
    return;
}

int main()
{
    int ret;
    struct sigaction act;
    act.sa_sigaction = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    if(sigaction(SIGINT, &act,NULL) < 0) 
        ERR_EXIT("sigaction");
    pid_t pid = fork();
    union sigval value;
    value.sival_int = 111;
    if(pid == 0) {
        for(int i = 0; i < 10;i++)
            if(sigqueue(getpid(),SIGINT,value)) 
                ERR_EXIT("sigqueue");
    }
    while(1) {
        do {
                ret = wait(NULL);
        }while(ret < 0 && errno == EINTR);
        sleep(1);
    }
    return 0;
}

