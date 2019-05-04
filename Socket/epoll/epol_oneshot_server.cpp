#include "model.h"

struct fds {
    int epollfd;
    int sockfd;
};
void set_noblock(int fd) {
    int flags;
    flags = fcntl(fd,F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd,F_SETFL,flags);
}

void add_fd(int epollfd,int fd,bool enable) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(enable) {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    set_noblock(fd);
}

int create_listenfd(int epollfd,int port,const char * ip) {
    struct sockaddr_in serv;
    memset(&serv,0,sizeof(serv));
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd < 0) 
        ERR_EXIT("socket");
    int flags = 1;
    socklen_t len = sizeof(flags);
    if(setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&flags,len) < 0) 
        ERR_EXIT("setsockopt");
    serv.sin_family = PF_INET;
    serv.sin_port = htons(port);
    inet_pton(AF_INET,ip,&serv.sin_addr);
    if(bind(lfd,(struct sockaddr*)&serv,sizeof(serv)) < 0) 
        ERR_EXIT("bind");
    if(listen(lfd,SOMAXCONN) < 0) 
        ERR_EXIT("listen");
    add_fd(epollfd,lfd,false);
    return lfd;
}
int create_connfd(int epollfd,int lfd) {
    struct sockaddr_in cli;
    memset(&cli,0,sizeof(cli));
    socklen_t len = sizeof(cli);
    int cfd = accept(lfd,(struct sockaddr*)&cli,&len);
    
    if(cfd < 0) 
        ERR_EXIT("accept");

    add_fd(epollfd,cfd,true);
    return cfd;
}
void reset_shot(int epollfd,int fd) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
void * worker (void * arg) {
    int sockfd = ((fds*)arg)->sockfd;
    int epollfd = ((fds*)arg)->epollfd;
    char buf[1024] = {0};
    while(1) {
          memset(buf,0,sizeof(buf));
          int ret = recv(sockfd,buf,sizeof(buf),0);
          if(ret < 0 ) {
            if(errno == EAGAIN) {
                cout<<"THe read has been over"<<endl;
                reset_shot(epollfd,sockfd);
                break;
                }
            }
            else if(ret == 0) {
                cout<<"The client is over"<<endl;
                close(sockfd);
                break;
            }
            else {
                send(sockfd,buf,sizeof(buf) - 1,0);
                continue;
            }
        }
    return NULL;
}
void et(struct epoll_event * events,int epollfd,int numbers,int lfd) {
    for(int i = 0;i< numbers;i++) {
        int sockfd = events[i].data.fd;
        if(sockfd == lfd) {
            int connfd = create_connfd(epollfd,lfd);
        }
        else if(events[i].events & EPOLLIN) {
            pthread_t ptd;
            struct fds tfd;
            tfd.epollfd = epollfd;
            tfd.sockfd = sockfd;
            pthread_create(&ptd,NULL,worker,&tfd);
        }
        else {
            cout<<"Something else is Over"<<endl;
        }
    }
}
int main(int argc,char * argv[]) {
    if(argc < 3) {
        cout<<"Please enter [./a.out] [port] [IP]"<<endl;
        exit(0);
    }
    int epollfd = epoll_create(5);
    int lfd = create_listenfd(epollfd,atoi(argv[1]),argv[2]);
    struct epoll_event events[100];
    while(1) {
        int ret = epoll_wait(epollfd,events,100,-1);
        if(ret < 0 ) 
            ERR_EXIT("epoll_wait");
        et(events,epollfd,ret,lfd);
    }
    close(lfd);
    return 0;
}

