#include "../model.h"
#include <netdb.h>

int main(int argc, char *argv[]) {
    int sockfd, confd;
    char buf[1024] = {0};
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    struct in_addr addr;
    struct in_addr **ptr;
    struct in_addr *addrp[2];
    struct hostent *hp;
    struct servent *sp;
    if (argc < 3) {
        cout << "参数错误" << endl;
    }
    if ((hp = gethostbyname(argv[1])) == NULL) {
        if (inet_aton(argv[1], &addr) == 0) {
            cout << "主机名错误：" << hstrerror(h_errno) << endl;
        }
        else {
            addrp[0] = &addr;
            addrp[1] = NULL;
            ptr = addrp;
        }
    }
    else {
        ptr = (struct in_addr **)hp->h_addr_list;
    }
    if((sp = getservbyname(argv[2],"tcp")) == NULL) {
        ERR_EXIT("getservbyname");
    }
    while (*ptr != NULL) {
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
            ERR_EXIT("socket");
        serv.sin_family = AF_INET;
        serv.sin_port = sp->s_port;
        memcpy(&serv.sin_addr,*ptr,sizeof(struct in_addr));
        char temp[1024];
        cout<<"Trying connect: "<<inet_ntop(AF_INET,&serv.sin_addr,temp,sizeof(temp))<<endl;
        socklen_t len = sizeof(serv);
        if(connect(sockfd,(struct sockaddr*)&serv,len) == 0)
            break;
        perror("connect");
        close(sockfd);
    }
    if(*ptr == NULL) {
        ERR_EXIT("Unable to connect");
    }
    int n;
    while((n = read(sockfd,buf,sizeof(buf))) > 0) {
        buf[n] = 0;
        fputs(buf,stdout);
        memset(buf,0,sizeof(buf));
    }
    exit(0);
    return 0;
}
