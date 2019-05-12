#include <iostream>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MEM_SIZE 10

using namespace std;
int main (int argc,char *argv[]) {
    char *addr;
    int fd;
    if(argc < 2 || strcmp(argv[1],"--help")  == 0) {
        cout<<"Index is Wrong"<<endl;
        exit(0);
    }
    if((fd = open(argv[1],O_RDWR)) < 0) {
        cout<<"open err"<<endl;
        exit(0);
    }
    addr = (char*)mmap(NULL,MEM_SIZE,
            PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    printf("Current = %.*s\n",MEM_SIZE,addr);
    if(argc > 2){
        if(strlen(argv[2]) >= MEM_SIZE) {
            cout<<"The String Is Too Long"<<endl;
            exit(0);
        }
        memset(addr,0,MEM_SIZE);
        strncpy(addr,argv[2],MEM_SIZE-1);
        if(msync(addr,MEM_SIZE,MS_SYNC) == -1) {
            cout<<"msync err"<<endl;
            exit(0);
        }
        printf("Copied \"%s\" to Memory\n",argv[2]);
    }
    exit(EXIT_SUCCESS);    
}