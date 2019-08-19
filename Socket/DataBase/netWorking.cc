#include "netWorking.h"

void NetWorking::acceptHandler() {
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DataStructure::Port);
    addr.sin_addr.s_addr = inet_addr(DataStructure::Ip.c_str());
    socklen_t len = sizeof(addr);
    confd_ = accept4(fd_,static_cast<struct sockaddr*>(boost::implicit_cast<void*>(&addr)),&len,SOCK_NONBLOCK);
    if(confd_ < 0) {
        std::cout << "[Accept Error]" << " " << __LINE__ << std::endl;
        int Errno = errno;
        switch (Errno) {
        case EAGAIN:
        case ECONNABORTED:
        case EINTR:
        case EPERM:
        case EMFILE: errno = Errno; break;
        case EINVAL:
        case ENOMEM:
        default:
            std::cout << "Unknown Error of Accept" << std::endl;
            break;
        }
    }
    char buf[1024] = {0};
    std::cout << "The Client has been Connected: " << inet_ntop(AF_INET,&addr.sin_addr,buf,sizeof(buf))
              << std::endl;

}