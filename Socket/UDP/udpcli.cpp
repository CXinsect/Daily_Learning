#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
using namespace std;
void echo_cli(int fd)
{
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(0);
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(8001);
    char recvbuf[1024] = {0};
    char sendbuf[1024] = {0};
    int ret;
    while(fgets(sendbuf,sizeof(sendbuf),stdin) != 0)
    {
        sendto(fd,"abcd",4,0,(struct sockaddr *)&servaddr,sizeof(servaddr));
        ret = recvfrom(fd,recvbuf,sizeof(recvbuf),0,0,0);
        if(ret == -1)
        {
            if(errno == EINTR)
                continue;
            exit(0);
        }
        fputs(recvbuf,stdout);
        memset(&sendbuf,0,sizeof(sendbuf));
        memset(&recvbuf,0,sizeof(recvbuf));
    }
    close(fd);
}
int main(void)
{
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        exit(0);
    }
    echo_cli(sockfd);
    return 0;
}
