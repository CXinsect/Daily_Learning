#include <sys/wait.h>
#include <iostream>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef USE_MAP_ANON

#define _BSD_OURCE

#endif

using namespace std;

int main (int argc,char * argv[]) {
    int *addr;
    #ifdef USE_MAP_ANON
        addr = (int *)mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE,
                                            MAP_ANONYMOUS | MAP_SHARED,-1,0);
        if(addr == NULL) {
            cout<<"mmap err"<<endl;
            exit(0);
        }
    #else
        int fd;
        fd = open("/dev/zero",O_RDWR);
        if(fd < 0) {
            cout<<"open err"<<endl;
            exit(0);
        }
        addr = (int *)mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE,
                                            MAP_SHARED,fd,0);
        if(addr == NULL) {
            cout<<"mmap err"<<endl;
            exit(0);
        } 
        close(fd);
    #endif
    *addr = 1;
    switch(fork()) {
        case 0:
            cout<<"Current Data: "<<*addr<<endl;
            (*addr)++;
            if(munmap(addr,sizeof(int)) < 0) {
                cout<<"Munmap err"<<endl;

            }
            exit(EXIT_SUCCESS);
        case -1:
            cout<<"fork err"<<endl;
            break;
        default:
            if(wait(NULL) == -1) {
                cout<<"wait err"<<endl;
                exit(0);
            }
            cout<<"Parent Value: "<<*addr<<endl;
            if(munmap(addr,sizeof(int)) < 0) {
                cout<<"Munmap err"<<endl;
            }
            break;
    }
    return 0;
}