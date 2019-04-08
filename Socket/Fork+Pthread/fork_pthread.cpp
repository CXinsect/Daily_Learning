#include "model.h"

pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;

void *do_action(void * arg) {
    pthread_mutex_lock(&mux);
    cout<<"I am Locked"<<endl;
    sleep(5);
    pthread_mutex_unlock(&mux);
    pthread_exit(0);
}
void prepare() {
    pthread_mutex_lock(&mux);
}
void infork() {
    pthread_mutex_unlock(&mux);
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,do_action,NULL);
    //让子线程占用互斥锁
    sleep(1);
    pthread_atfork(prepare,infork,infork);
    pid_t pid = fork();
    if(pid < 0) {
        pthread_join(tid,NULL);
        pthread_mutex_destroy(&mux);
    }
    else if(pid == 0) {
        pthread_mutex_lock(&mux);
        cout<<"Let me out"<<endl;
        pthread_mutex_unlock(&mux);
    }
    else {
        pid_t pid;
        while( (pid = waitpid(-1,NULL,WNOHANG)) != -1)
            cout<<"over: "<<pid<<endl;
    }
    pthread_join(tid,NULL);
    pthread_mutex_unlock(&mux);

    return 0;
}

