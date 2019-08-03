#include <string.h>
#include <sys/mman.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main (int argc,char *argv[]) {
    int fd = open(argv[1],O_RDONLY);
    std::cout << fd << std::endl;
    struct stat st;
    if(stat(argv[1],&st) < 0)
        exit(0);
    char * addr = (char*)mmap(NULL,st.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    if(addr == MAP_FAILED) {
        std::cout << "mmap err" << std::endl;
    }
    close(fd);
    std::cout << addr << std::endl;
    munmap(addr,st.st_size);
    return 0;
}