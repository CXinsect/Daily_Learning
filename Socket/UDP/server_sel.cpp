#include "model.h"
int create_lfd(void) {
    struct sockaddr_in serv;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0) 
        ERR_EXIT("sockfd");
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd,(struct sockaddr*)&serv,sizeof(serv)) < 0)
        ERR_EXIT("bind");
    if(listen(sockfd,SOMAXCONN) < 0)
        ERR_EXIT("listen");
    return sockfd;
}
int create_ufd(void) {
    struct sockaddr_in serv;
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0) 
        ERR_EXIT("sockfd");
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd,(struct sockaddr*)&serv,sizeof(serv)) < 0)
        ERR_EXIT("bind");
    return sockfd;
}
void handler(int i) {
    while(waitpid(-1,NULL,WNOHANG) < 0)
        ;
    cout<<"Hello,World"<<endl;
}
void do_action(int conn){
    while(1){
        char recvbuf[1024] = {0};
        int ret = read(conn,recvbuf,sizeof(recvbuf));
        if(ret == -1)
        exit(0);
        if(ret == 0) {
            cout<<"client is over"<<endl;
            exit(0);
        }
        fputs(recvbuf,stdout);
        write(conn,recvbuf,strlen(recvbuf));
        memset(recvbuf,0,sizeof(recvbuf));
    }
}
void udp_action(int ufd) {
                struct sockaddr_in serv;
                memset(&serv,0,sizeof(serv));
                socklen_t len = sizeof(serv);
                char buf[1024] = {0};
                while(1) {
                     memset(buf,0,sizeof(buf));
                     recvfrom(ufd,buf,sizeof(buf),0,(struct sockaddr*)&serv,&len);
                     sendto(ufd,buf,strlen(buf),0,(struct sockaddr*)&serv,sizeof(serv));
                }
}
int main()
{
    signal(SIGCHLD,handler);
    fd_set allset;
    FD_ZERO(&allset);
    int lfd = create_lfd();
    int ufd = create_ufd();
    int maxfd = max(lfd,ufd) + 1;
    int ret,cfd;
    while(1) {
        FD_SET(lfd,&allset);
        FD_SET(ufd,&allset);
        do {
            ret = select(maxfd,&allset,NULL,NULL,NULL);
        }while(ret < 0 && errno == EINTR);
        if(ret == 0)
            continue;
        else if(ret < 0)
            ERR_EXIT("select");
        else {
                    if(FD_ISSET(ufd,&allset)) {
                        pid_t pid = fork();
                        if(pid == 0) 
                        {
                            udp_action(ufd);
                            exit(0);
                        }
                    }
                    if(FD_ISSET(lfd,&allset)) {
                        struct sockaddr_in cli;
                        memset(&cli,0,sizeof(cli));
                        socklen_t len = sizeof(cli);
                        cfd = accept(lfd,(struct sockaddr*)&cli,&len);
                        cout<<"New Client"<<endl;
                        if(cfd  < 0)
                            ERR_EXIT("accept");
                        pid_t pid = fork();
                        if(pid == 0) {
                            close(lfd);
                            do_action(cfd);
                            exit(0);
                        }
                        close(cfd);
                    }
         }
    }
    return 0;
}

