/*************************************************************************
	> File Name: echo_cli.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年06月16日 星期日 11时57分22秒
 ************************************************************************/
#include "model.h"

int  connect(int port) {
    struct sockaddr_in  serv;
    memset(&serv,0,sizeof(serv));
    int sockfd;
    if((sockfd = socket(PF_INET,SOCK_STREAM,0)) < 0) 
        ERR_EXIT("socket");
    serv.sin_family = PF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = 0;
    if(connect(sockfd,(struct sockaddr *)&serv,sizeof(serv)))
        ERR_EXIT("connect");
    return sockfd;
}
int main(int argc,char * argv[])
{
    if(argc < 2) {
        cout<<"The index is wrong"<<endl;
        exit(0);
    }
    int sockfd = connect(atoi(argv[1]));
    char buf[1024] = {0};
    char recvbuf[1024] = {0};
    char sendbuf[1024] = "hello,Linux";
    int nwrite = write(sockfd,sendbuf,strlen(buf));
    std::cout << nwrite << std::endl;
    int n;
    while((n = read(sockfd,recvbuf,sizeof(recvbuf)) >= 0)) {
        std::cout << recvbuf << "n: "<< n <<  std::endl;
        if(n == 0) {
            std::cout << "Server is Over" << std::endl;
            break;
        }
        memset(recvbuf,0,sizeof(recvbuf));
    }
    close(sockfd);
    return 0;
}
