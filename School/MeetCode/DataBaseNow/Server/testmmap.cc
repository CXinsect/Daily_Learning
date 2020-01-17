#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

int main (void) {
    int fd = open("1.txt",O_CREAT | O_RDWR,0666);
    ftruncate(fd,23);
    assert(fd != -1);
    struct stat st;
    int off = 0;
    stat("1.txt",&st);
    char* m = static_cast<char*>(mmap(NULL,st.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0));
    // close(fd);
    if(m == MAP_FAILED) std::cout << strerror(errno) << std::endl; 
    char buf[12] = "hello,world";
    int count = 2;
    while(count--) {
        memcpy(m + off,buf,strlen(buf));
        off += strlen(buf);
    }
    
    munmap(m,st.st_size);
    return 0;
}