#include "Accept.h"
Accept::Accept(EventLoop* loop, const Address& listenAddr):
                                    loop_(loop),
                                    acceptSocket_(sockets::creatNonblocking(AF_INET)),
                                    acceptChannel_(loop,acceptSocket_.getSockfd()),
                                    listening_(false)
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Accept::handleRead,this));
}
void Accept::listen() {
    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}
void Accept::handleRead () {
    Address peerAddr(0,false);
    int confd = acceptSocket_.accept(&peerAddr);
    if(confd >= 0) {
        if(connectioncallback_)
            connectioncallback_(confd,peerAddr);
        else {
            sockets::close(confd);
        }
    }
}