#pragma once

#include "model.h"
 #include <sys/ipc.h>

//错误码
#define ERR_BASE 100
#define ERR_EEXIST (ERR_BASE + 1)
#define ERR_PRAGRAM (ERR_BASE + 2)
//共享内存
int IPC_CreateShm(int key,int size,int *shmfd);
int IPC_MapShm(int shmfd, void **mapaddr);
int IPC_UnMapShm(void * unmapaddr);
int DelShm(int shmfd);
//信号量
int IPC_CreateSem(int key,int nums,int *semfd);
int IPC_OpenSem(int key,int *semfd);
int IPC_GetVal(int semfd,int * val);
int IPC_SetVal(int semfd,int val);
int IPC_Sem_P(int semfd);
int IPC_Sem_V(int semfd);
