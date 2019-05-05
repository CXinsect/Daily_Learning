#include "get_num.cpp"
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CMD_SIZE 200
#define BUF_SIZE 1024

using namespace std;

int main (int argc,char *argv[]) {
    int fd,j,blocks;
    char cmd[CMD_SIZE] = {0};
    char buf[BUF_SIZE] = {0};
    if(argc < 2 || strcmp(argv[1],"--help") == 0) {
        cout<<"pragma err"<<endl;
    }
    blocks = (argc > 2) ? getInt(argv[2],GN_GT_0,"1kb-blocks") : 100000;
    fd = open(argv[1],O_WRONLY | O_CREAT | O_EXCL,S_IRUSR | S_IWUSR);
    if(fd < 0) {
        cout<<"open err: "<<strerror(errno)<<endl;
        exit(0);
    }
    if(unlink(argv[1]) == -1) {
        cout<<"unlink err"<<endl;
        exit(0);
    }
    for(j = 0;j < blocks;j++) {
        if(write(fd,buf,sizeof(buf)) != BUF_SIZE) {
            cout<<"write err"<<endl;
            exit(0);
        }
    }
    snprintf(cmd,CMD_SIZE,"df -k `dirname %s`",argv[1]);
    system(cmd);
    if(close(fd) == -1) {
        cout<<"close err"<<endl;
        exit(0);
    }
    cout<<"The file has been closed"<<endl;
    system(cmd);
    exit(EXIT_SUCCESS);
    return 0;
}