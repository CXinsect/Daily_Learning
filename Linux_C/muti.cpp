#include "head.h"

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                           (Linux specific) */
};
int IPC_CreateShm(int key,int size,int *shmfd) {
    
    *shmfd = shmget(key,size,0666|IPC_CREAT);
    if(*shmfd == -1) {
        return -2;
    }
    return 0;
}
int IPC_MapShm(int shmfd, void **mapaddr) {
    void *ptr = NULL;
    ptr = (void*)shmat(shmfd,0,SHM_RND);
    if(*(int*)ptr == -1) 
        return -1;
    *mapaddr = ptr;
    return 0;
}
int IPC_UnMapShm(void * unmapaddr) {
    int ret;
    ret = shmdt((char*)unmapaddr);
    if(ret == -1) 
        return -1;
    return 0;
}
int DelShm(int shmfd) {
    int ret;
    ret = shmctl(shmfd,IPC_RMID,NULL);
    if(ret < 0)
        return -1;
    return 0;
}
int IPC_CreateSem(int key,int nums,int *semfd) {
    int ret = semget(key,nums,0666|IPC_CREAT|IPC_EXCL);
    if(ret == -1) {
        if(errno == EEXIST) {
            errno = ERR_EEXIST;
            return ret;
        }
    }
    *semfd = ret;
    ret = 0;
    return ret;
}
int IPC_OpenSem(int key,int *semfd) {
   int ret = 0;
   if(semfd == NULL) {
       ret = ERR_PRAGRAM;
       cout<<"func: sem_open()"<<endl;
       return ret;
   }
   ret = semget(key,0,0);
   if(ret == -1) {
       cout<<"sem_open func"<<endl;
       return ret;
   }
   *semfd = ret;
   ret = 0;
   return ret;

}
int IPC_SetVal(int semfd,int val) {
    int ret = 0;
    union semun sem;
    sem.val = val;
    ret = semctl(semfd,0,SETVAL,sem);
    cout<<"set"<<sem.val<<endl;
    return ret;
}
int IPC_GetVal(int semfd,int * val) {
    union semun sem;
    *val = sem.val;
    int ret = semctl(semfd,0,GETVAL,sem);
    *val = ret;
    ret = 0;
    return ret;
}
int IPC_Sem_P(int semfd){
    struct sembuf buf = {0,-1,0};
    int ret = 0;
    ret = semop(semfd,&buf,1);
    return ret;
}
int IPC_Sem_V(int semfd) {
    struct sembuf buf = {0,1,0};
    int ret = 0;
    ret = semop(semfd,&buf,1);
    return ret;
}
