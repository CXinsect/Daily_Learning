#include "model.h"

void* do_action(void *arg) {
    sigset_t *sset = (sigset_t *)arg;
    int ret,sig;
    while(1) {
        ret = sigwait(sset,&sig);
        if(ret != 0) 
            ERR_EXIT("sigwait");
        cout<<"capture: "<<sig<<endl;
    }
    pthread_exit(0);
}
int main()
{
    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset,SIGQUIT);
    sigaddset(&sset,SIGUSR1);
    pthread_t  tid;
    int ret = pthread_sigmask(SIG_BLOCK,&sset,NULL);
    if(ret != 0)
        ERR_EXIT("pthread_sigmask");
    ret = pthread_create(&tid,NULL,do_action,(void *)&sset);
    if(ret != 0)
        ERR_EXIT("pthread_create");
    pause();
    return 0;
}

