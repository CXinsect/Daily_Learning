#include "../model.h"
#include <netdb.h>
#include <sys/un.h>
#include <iostream>

//从套接字地址结构中取出ip地址和端口号
char *sock_ntop(const struct sockaddr *sa, socklen_t salen) {
    
    char portstr[8];    
    static char str[128];
   
    switch (sa->sa_family) {
        case AF_INET: {
            struct sockaddr_in    *sin = (struct sockaddr_in *) sa;

            if (inet_ntop(AF_INET, &sin->sin_addr, str, 
                sizeof(str)) == NULL) {
                return(NULL);
            }
            if (ntohs(sin->sin_port) != 0) {
                snprintf(portstr, sizeof(portstr), ":%d", 
                    ntohs(sin->sin_port));
                strcat(str, portstr);
            }
            return(str);
        }
        case AF_INET6: {
            struct sockaddr_in6    *sin6 = (struct sockaddr_in6 *) sa;

            str[0] = '[';
            if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, 
                sizeof(str) - 1) == NULL) {
                return(NULL);
            }
            if (ntohs(sin6->sin6_port) != 0) {
                snprintf(portstr, sizeof(portstr), "]:%d",
                    ntohs(sin6->sin6_port));
                strcat(str, portstr);
                return(str);
            }
            return (str + 1);
        }
        case AF_UNIX: {
            struct sockaddr_un    *unp = (struct sockaddr_un *) sa;

            if (unp->sun_path[0] == 0) {
                strcpy(str, "(no pathname bound)");
            } else {
                snprintf(str, sizeof(str), "%s", unp->sun_path);
            }
            return(str);
        }
        default: {
            snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
                 sa->sa_family, salen);
            return(str);
        }
    }
    return (NULL);
}
int tcp_connect (const char *name,const char *service) {
    int sockfd;
    int n;
    struct addrinfo hints,*res,*resault;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if((n = getaddrinfo(name,service,&hints,&res)) != 0) {
        ERR_EXIT("getaddrinfo");
    }
    resault = res;
    do {
        //创建套接字地址结构
        sockfd = socket(res->ai_flags,res->ai_socktype,res->ai_protocol);
        if(sockfd < 0) {
            continue;
        }
        //连接返回的地址列表中的ip地址
        if(connect(sockfd,res->ai_addr,res->ai_addrlen) == 0) {
            break;
        }
        close(sockfd);
    }while((res = res->ai_next) != NULL);
    if(res == NULL) {
        ERR_EXIT("tcp_connect");
    }
    freeaddrinfo(resault);
    return sockfd;
}
int main (int argc,char *argv[]) {
    if(argc < 3) {
        cout<<"参数错误"<<endl;
        exit(0);
    }
    int sockfd;
    int n;
    char recvbuf[1024] = {0};
    socklen_t len;
    struct sockaddr_storage ss;
    //已连接套接字
    sockfd = tcp_connect(argv[1],argv[2]);
    len = sizeof(ss);
    //获取套接字相关的外地协议地址
    if(getpeername(sockfd,(struct sockaddr*)&ss,&len) < 0)
        ERR_EXIT("getpeername");
    cout<<"connected: "<<sock_ntop((struct sockaddr*)&ss,len)<<endl;
    while((n = read(sockfd,recvbuf,sizeof(recvbuf))) > 0) {
        recvbuf[n] = 0;
        fputs(recvbuf,stdout);
        memset(recvbuf,0,sizeof(recvbuf));
    }
    return 0;
}