#include "muti.cpp"
int key = 0x1234;
int S_key = 0x6666;
void do_action(int loop) {
    cout<<"loop: "<<loop<<endl;
    int shmfd = 0,semfd = 0;
    int ret = 0,ncount = 0;
    int *addr = NULL;
    const char * p = ".";
    ret = IPC_OpenSem(key,&semfd);
    IPC_Sem_P(semfd);
        ret = IPC_CreateShm(S_key,0,&shmfd);
        ret = IPC_MapShm(shmfd,(void**)&addr);
        *((int*)addr) = *((int*)addr) + 1;
        ncount = *((int*)addr);
        cout<<"ncount: "<<ncount<<endl;
        ret = IPC_UnMapShm(addr);
        sleep(1);
    IPC_Sem_V(semfd);
    cout<<"进程退出： "<<getpid()<<endl;
}
int main(void)
{
    int semfd = 0,shmfd;
    IPC_CreateShm(S_key,4,&shmfd);
    if((IPC_CreateSem(key,1,&semfd)) < 0) {
        if(errno == ERR_EEXIST) {
            IPC_OpenSem(key,&semfd);
            cout<<"信号量已经存在"<<endl;
        }
        else
            return -1;
    }
    IPC_SetVal(semfd,1);
    cout<<"进程数: 循环数"<<endl;
    int pnums,lnums;
    cin>>pnums>>lnums;
    cout<<"进程数: "<<pnums<<"循环数: "<<lnums<<endl;
    int temp = 0;
    if(IPC_GetVal(semfd,&temp) < 0) {
        cout<<"Error"<<endl;
    }
    cout<<"val: "<<temp<<endl;
    for(int i = 0;i < pnums;i++) {
        pid_t pid = fork();
        if(pid == 0) {
            for(int j = 0;j < lnums; j++)
                do_action(j);
            exit(0);
        }
    }
    return 0;
}

