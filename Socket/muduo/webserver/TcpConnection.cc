#include "TcpConnection.h"


void TcpConnection::handleRead () {
    char buf[65535];
    ssize_t n = ::read(channel_->getFd(),buf,sizeof(buf));
    if(n > 0)
        messageCallBack_(shared_from_this(),buf,static_cast<int>(n));
    else if(n == 0)
        handClose();
    else {
        std::cout << __FILE__ << "err" << std::endl;
        exit(0);
    }
}
void TcpConnection::handClose () {
    std::cout << "handleState: " << state_ << std::endl;
    assert(state_ == Connected);
    channel_->disableAll();
    closeCallBack_(shared_from_this());
}

