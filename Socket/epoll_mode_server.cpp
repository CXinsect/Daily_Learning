#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#define ERR_EXIT(m) \
 do { \
    perror(m); \
    exit(EXIT_FAILURE); \
}while(0)
using namespace std;
void set_noblock(int fd) {
    int flags;
    int ret;
    flags = fcntl(fd,F_GETFL);
    if(flags < 0) 
        ERR_EXIT("fcntl");
    flags |= O_NONBLOCK;
    ret = fcntl(fd,F_SETFL,flags);
    if(ret < 0) 
        ERR_EXIT("fcntl");
}
void add_connfd(int epollfd,int fd,bool enable) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if(enable) {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    set_noblock(fd);
}

int create_listenfd(int epollfd,int port, char *ip) {
    struct sockaddr_in serv;
    int listenfd;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd < 0) 
        ERR_EXIT("socket");
    int flags = 1;
    socklen_t len = sizeof(flags);
    
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&flags,len) < 0) 
        ERR_EXIT("setsockopt");
    
    memset(&serv,0,sizeof(serv));
    serv.sin_family = PF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;
   // inet_pton(AF_INET,ip,&serv.sin_addr);
    if(bind(listenfd,(struct sockaddr *)&serv,sizeof(serv)) < 0) 
       ERR_EXIT("bind");
    
    if(listen(listenfd,SOMAXCONN) < 0)
        ERR_EXIT("listen");
    add_connfd(epollfd,listenfd,true);
    return listenfd;
}


void create_connfd(int epollfd,int listenfd,bool enable) {
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int connfd;
    if((connfd = accept(listenfd,(struct sockaddr *)&cli,&len)) < 0) 
        ERR_EXIT("accept");
    cout<<"well"<<endl;
    add_connfd(epollfd,connfd,enable);
    cout<<"hello"<<endl;
}
//number 表示返回的就绪个数
void lt(epoll_event * events,int number,int epollfd,int listenfd) {
    char buf[1024];
    for(int i = 0;i<number;i++) {
        int sockfd = events[i].data.fd;
        if(sockfd == listenfd) {
                create_connfd(epollfd,listenfd,false);
        }
        else if(events[i].events & EPOLLIN) {
            memset(buf,0,sizeof(buf));
            int ret = recv(sockfd,buf,sizeof(buf)-1,0);
            if(ret < 0) {
                ERR_EXIT("recv");
                close(sockfd);
                continue;
            }
            else if(ret == 0) {
                cout<<"The client has been closed"<<endl;
                close(sockfd);
                continue;
            }
            else {
                cout<<"sleep"<<endl;
                cout<<buf<<endl;
                send(sockfd,buf,sizeof(buf),0);
                //continue;
                close(sockfd);
            }
        }
    }
}
void et(epoll_event * events,int number,int epollfd,int listenfd) {
    char buf[1024];
    for(int i = 0;i<number;i++) {
        int sockfd = events[i].data.fd;
        if(sockfd == listenfd) {
            create_connfd(epollfd,listenfd,true);
        }
        else if(events[i].events & EPOLLIN) {
            
            while(1) {
                memset(buf,0,sizeof(buf));
                int ret = recv(sockfd,buf,sizeof(buf)-1, 0);
                if(ret < 0) {
                    if(errno == EAGAIN || errno == EWOULDBLOCK) {
                        cout<<"Read later"<<endl;
                        break;
                    }
                    close(sockfd);
                    break;
                } 
                else if(ret == 0) {
                    cout<<"The client is over"<<endl;
                    break;
                }
                else {
                    cout<<"Doing Things"<<endl;
                }
                send(sockfd,buf,sizeof(buf),0);
            }
        }
        else {
            cout<<"Something else"<<endl;
        }
    }
}
int main(int argc,char * argv[])
{
    if(argc < 3) {
        cout<<"The index is Wrong"<<endl;
        exit(0);
    }
    int epollfd = epoll_create(10);
    int port = atoi(argv[1]);
    int listenfd = create_listenfd(epollfd,port,argv[2]);
    struct epoll_event events[10];
    while(1) {
        int ret = epoll_wait(epollfd,events,10,-1);
        if(ret < 0)
            break;
        et(events,ret,epollfd,listenfd);
    }
    close(listenfd);
    return 0;
}

