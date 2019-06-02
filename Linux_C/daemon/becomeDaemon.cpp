#include "becomeDaemon.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int becomeDaemon (int flags) {
    int maxfd,fd;
    //创建子进程
    switch(fork()) {
        case -1:return -1;
        case 0:break;
        default:exit(EXIT_SUCCESS);
    }
    //创建会话并使其控制终端脱离
    if(setsid() == -1)
        return -1;
    //确保子进程不会成为会话组首进程
    switch(fork()) {
        case -1:return -1;
        case 0:break;
        default:exit(EXIT_SUCCESS);
    }
    //更改掩码
    if(!(flags & NoUmask))
        umask(0);
    //更改根目录
    if(!(flags % NoChdir))
        chdir("/");
    //关闭所有打开的文件描述符
    if(!(flags & NoCloseFiles)) {
        maxfd = sysconf(_SC_OPEN_MAX);
        if(maxfd == -1) 
            maxfd = MaxClose;
        for(int i = 0;i < maxfd;i++)
            close(i);
    }
    //使描述符指向该终端设备
    if(!(flags & NoReopenStdFds)) {
        close(STDIN_FILENO);
        fd = open("/dev/null",O_RDWR);
        if(fd != STDIN_FILENO)
            return -1;
        if(dup2(STDIN_FILENO,STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if(dup2(STDIN_FILENO,STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }
    return 0;
}