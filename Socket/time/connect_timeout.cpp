#include "model.h"

int set_noblock(int fd) {
    int flags;
    flags = fcntl(fd,F_GETFL);
    int tag = flags | O_NONBLOCK;
    fcntl(fd,F_SETFL,tag);
    return flags;
}

int create_connfd(int port,int t) {
    int connfd = socket(AF_INET,SOCK_STREAM,0);
    if(connfd < 0) 
        ERR_EXIT("socket");
    struct sockaddr_in serv;
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;
    int flags = set_noblock(connfd);
    int ret = connect(connfd,(struct sockaddr *)&serv,sizeof(serv));
    if(ret == 0) {
        cout<<"connsected"<<endl;
        fcntl(connfd,F_SETFL,flags);
        return connfd;
    }
    else if(errno != EINPROGRESS) {
        cout<<"connnsect failed"<<endl;
        return -1;
    }
    fd_set wd;
    FD_ZERO(&wd);
    FD_SET(connfd,&wd);
    struct timeval tv;
    tv.tv_sec = t;
    tv.tv_usec = 0;
    ret = select(connfd+1,NULL,&wd,NULL,&tv);
    if(ret <= 0) {
        cout<<"connect time out"<<endl;
        close(connfd);
        return -1;
    }
    else if(!FD_ISSET(connfd,&wd)) {
        cout<<"no events exist"<<endl;
        close(connfd);
        return -1;
    }
    int err = 1;
    socklen_t len = sizeof(err);
    ret = getsockopt(connfd,SOL_SOCKET,SO_ERROR,&err,&len);
    if(ret < 0) {
        cout<<"getsockopt err"<<endl;
        close(connfd);
        return -1;
    }
    if(err !=  0) {
        cout<<"Something is Wrong"<<endl;
        close(connfd);
        return -1;
    }
    cout<<"The road is open"<<endl;
    fcntl(connfd,F_SETFL,flags);
    return connfd;
}
int main(int argc,char * argv[])
{
    if(argc < 2) {
        cout<<"Please Enter [./a.out] [port]"<<endl;
        exit(0);
    }
    int connfd = create_connfd(atoi(argv[1]),10);
    char buf[1024] = {0};
    char recvbuf[1024] = {0};
    do {
        if(connfd == -1)
            break;
        while(fgets(buf,sizeof(buf),stdin) != NULL) {
            send(connfd,buf,sizeof(buf)-1,0);
            recv(connfd,recvbuf,sizeof(recvbuf),0);
            cout<<"buf: "<<buf<<endl;
            memset(buf,0,sizeof(buf));
            memset(recvbuf,0,sizeof(recvbuf));
        }
    }while(0);
    if(connfd != -1)
        close(connfd);
    return 0;
}


