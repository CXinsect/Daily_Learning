#include "timer.cpp"

#define MAX_CONN 1024
#define BUFFIZE 1024
#define FD_LIMIT 65535
#define TIMELOT 5
List list_;
static int pipefd[2];
int epollfd = 0;

struct packet {
    struct sockaddr_in address;
    int sockfd;
};

int set_noblock(int fd) {
    int flags;
    flags = fcntl(fd,F_GETFL);
    flags |= O_NONBLOCK;
    int newfd = fcntl(fd,F_SETFL,flags);
    return flags;
}
void add_fd (int epollfd,int fd) {
    struct epoll_event events;
    events.data.fd = fd;
    events.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&events);
    set_noblock(fd);
}
int create_listenfd(int epollfd,int port) {
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
    add_fd(epollfd,listenfd);
    return listenfd;
}
struct packet * create_connfd(int epollfd,int listenfd) {
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int connfd;
    if((connfd = accept(listenfd,(struct sockaddr *)&cli,&len)) < 0)
        ERR_EXIT("accept");
    add_fd(epollfd,connfd);
    struct packet * pack = new (struct packet);
    pack->address = cli;
    pack->sockfd = connfd;
    return pack;
}
void sig_handler(int sig) {
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1],(char*)&msg,1,0);
    errno = save_errno;
}
void add_sig(int sig) {
    struct sigaction sa;
    memset(&sa,0,sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    if(sigaction(sig,&sa,NULL) < 0) 
        ERR_EXIT("sigaction");
}
void time_handle() {
    list_.tick();

    alarm(TIMELOT);
}
void func(struct client_data *data) {
    epoll_ctl(epollfd,EPOLL_CTL_DEL,data->sockfd,0);
    
    close(data->sockfd);
}
void work(struct epoll_event * events,int numbers,int epollfd,int lfd,
          struct client_data *user,
          bool &timeout,
          bool &stop_service) {
    for(int i = 0;i < numbers;i++) {
        int sockfd = events[i].data.fd;
        if(sockfd == lfd) {
            struct packet *pack = create_connfd(epollfd,lfd);
            user[pack->sockfd].address = pack->address;
            user[pack->sockfd].sockfd = pack->sockfd;
            //创建并绑定定时器，然后将定时器插入链表
            U_timer * tmp = new U_timer;
            time_t cur = time(NULL);
            tmp->expire = cur + 3 * TIMELOT;
            tmp->user_data = &user[pack->sockfd];
            tmp->func = func;
            user[pack->sockfd].timer = tmp;
            list_.add_timer(tmp);
            delete pack;
        }
        else if((sockfd == pipefd[0]) && (events[i].events & EPOLLIN)) {
            char sigbuf[1024];
            int ret = recv(sockfd,sigbuf,sizeof(sigbuf),0);
            if(ret < 0)
                continue;
            else if(ret == 0)
                continue;
            else {
                for(int i = 0;i < ret;i++) {
                    switch(sigbuf[i]) {
                    case SIGALRM:{
                                     timeout = true;
                                     break;
                                 }
                    case SIGTERM:
                                 stop_service = true;
                    }
                }
            }
        }
        else if(events[i].events & EPOLLIN) {
            U_timer * tmp = user[sockfd].timer;
            memset(user[sockfd].buf,0,sizeof(user[sockfd].buf));
            int ret = recv(sockfd,user[sockfd].buf,sizeof(user[sockfd].buf),0);
            if(ret < 0) {
                
                if(errno != EAGAIN) {
                    tmp->func(&user[sockfd]);
                    if(tmp) {
                        list_.del_timer(tmp);
                    }
                }
            }
            else if(ret == 0) {
                tmp->func(&user[sockfd]);
                if(tmp) {
                    list_.del_timer(tmp);
                }
            }
            else {
                if(tmp) {
                    time_t cur = time(NULL);
                    tmp->expire = cur + 3 * TIMELOT;
                    list_.adjust_timer(tmp);
                    send(sockfd,user[sockfd].buf,sizeof(user[sockfd].buf)-1,0);
                    cout<<"The data has been sent"<<endl;
                }
            }

        }
        else {
                cout<<"Other Things"<<endl;
        }
    }
}
int main (int argc,char * argv[]) {
    if(argc < 3) {
        cout<<"Please enter [./a.out] [port] [IP]"<<endl;
        return -1;
    }
    epollfd = epoll_create(5);
    int port = atoi(argv[1]);
    int lfd = create_listenfd(epollfd,port);
    int ret = socketpair(PF_UNIX,SOCK_STREAM,0,pipefd);
    if(ret < 0) 
        ERR_EXIT("socketpair");
    set_noblock(pipefd[1]);
    add_fd(epollfd,pipefd[0]);
    add_sig(SIGALRM);
    add_sig(SIGTERM);
    bool timeout = false;
    bool stop_service = false;
    struct epoll_event events[MAX_CONN];
    struct client_data * user = new struct client_data[FD_LIMIT];
    alarm(TIMELOT);
    while(!stop_service) {
        do {
            ret = epoll_wait(epollfd,events,MAX_CONN,-1);
        }while(ret < 0 && errno == EINTR);
        work(events,ret,epollfd,lfd,user,timeout,stop_service);
        if(timeout) {
            time_handle();
            timeout = false;
        }
    }
    close(pipefd[1]);
    close(pipefd[0]);
    close(lfd);
    delete [] user;
    return 0;
}
