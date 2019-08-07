#include "Acceptor.h"
#include "Socket.cc"
using namespace sockets;
Acceptor::Acceptor(EventLoop* loop, const Address& listenAddr)
    : loop_(loop),
      acceptSocket_(sockets::creatNonblocking(AF_INET)),
      acceptChannel_(loop, acceptSocket_.getSockfd()),
      listening_(false) {
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.bindAddress(listenAddr);
  acceptChannel_.setReadCallBack(bind(&Acceptor::handleRead, this));
}
void Acceptor::Listen() {
  listening_ = true;
  acceptSocket_.listen();
  acceptChannel_.enableReading();
  // sleep(500);
}

void Acceptor::handleRead() {
  Address perrAddr(0);
  int confd = acceptSocket_.accept(&perrAddr);
  struct pollfd pfd;
  pfd.fd = confd;
  pfd.events = POLLRDNORM;
  if (confd >= 0) {
      if (newConnectionBack_)
        newConnectionBack_(confd, perrAddr);
      else
        sockets::close(confd);
    }
    else {
        if(errno == EMFILE)
            ::close(confd);
    }
}
