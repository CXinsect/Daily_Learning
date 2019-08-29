#include "Client.h"

int Client::setNoOrBlocking (int sockfd) {
    int ret = ::fcntl(sockfd,F_GETFL);
    assert(ret != -1);
    int org = ret | O_NONBLOCK;
    int res = ::fcntl(sockfd,F_SETFL,ret);
    assert(res != -1);
    return ret;
}
int Client::Connect() {
    int sockfd = ::socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd != -1);
    struct sockaddr_in serv;
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = port_;
    serv.sin_addr.s_addr = ::inet_addr(Ip_.c_str());
    int original = setNoOrBlocking(confd_);
    confd_ = ::connect(sockfd,(struct sockaddr*)&serv,sizeof(serv));
    if(confd_ < 0 && errno != EINPROGRESS) {
        ::close(confd_);
        return -1;
    }
    else if(confd_ == 0) {
        int res = ::fcntl(confd_,F_SETFL,original);
        assert(res != -1);
        return confd_;
    }
        
    fd_set wd;
    FD_ZERO(&wd);
    FD_SET(confd_,&wd);
    struct timeval tv;
    tv.tv_sec = time_;
    int ret = ::select(confd_+1,NULL,&wd,NULL,&tv);
    if(ret <= 0) {
        std::cout << "Connect timeout" << std::endl;
        ::close(confd_);
        return -1;
    }
    else if(!FD_ISSET(confd_,&wd)) {
        std::cout << "No Active Event" << std::endl;
        ::close(confd_);
        return -1;
    }
    int err;
    socklen_t len = sizeof(err);
    ret = ::getsockopt(confd_,SOL_SOCKET,SO_ERROR,&err,&len);
    assert(ret >= 0);
    if(err != 0) {
        std::cout << "Socket Wrong" << std::endl;
        ::close(confd_);
        return -1;
    }
    ret = ::fcntl(confd_,F_SETFL,original);
    assert(ret != -1);
    return confd_;
}