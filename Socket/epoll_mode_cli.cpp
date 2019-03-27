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
    while(fgets(buf,sizeof(buf),stdin) != NULL) {
        send(sockfd,buf,sizeof(buf) - 1,0);
        recv(sockfd,recvbuf,sizeof(recvbuf),0);
        cout<<recvbuf<<endl;
        memset(buf,0,sizeof(buf));
        memset(recvbuf,0,sizeof(recvbuf));
    }
    close(sockfd);
    return 0;
}

