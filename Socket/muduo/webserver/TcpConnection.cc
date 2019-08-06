#include "TcpConnection.h"

void TcpConnection::handleRead() {
  std::cout << "TcpConnnection handleread" << std::endl;
  int n = static_cast<int>(inputBuffer_.readFd(channel_->getFd()));

  if (n > 0) {
    messageCallBack_(shared_from_this(), &inputBuffer_);
  } else if (n == 0) {
    std::cout << "client is over" << std::endl;
    handClose();
  } else {
    std::cout << "TcpConnection Error:: handRead" << std::endl;
    handClose();
  }
}
void TcpConnection::handWrite() {
  if (channel_->isWriteing()) {
    std::cout << "writeing" << std::endl;
    ssize_t n = ::write(channel_->getFd(), outputBuffer_.peek(),
                        outputBuffer_.getReadableBytes());
    if (n > 0) {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.getReadableBytes() == 0) {
        channel_->disableWriteing();
        if (state_ == Disconnecting) shutdownInLoop();
      }
    }
  } else {
    std::cout << "Connection is Down" << std::endl;
  }
}
void TcpConnection::handClose() {
  assert(state_ == Connected || state_ == Disconnecting);
  setState(Disconnceted);
  channel_->disableAll();
  closeCallBack_(shared_from_this());
}
void TcpConnection::connectionClose() {
  assert(state_ == Connected);
  setState(Disconnceted);
  channel_->disableAll();
  connectionCallBack_(shared_from_this());
  channel_->remove();
}
void TcpConnection::send(const std::string& message) {
  if (state_ == Connected) {
    sendInLoop(message);
  }
}
void TcpConnection::send(Buffer * buffer) {
  if(state_ == Connected) {
    sendInLoop(buffer->retrieveAllAsString());
  }
}
void TcpConnection::sendInLoop(const std::string& message) {
  ssize_t nwrite = 0;
  std::cout << "Sendinloop:: Messgae : " << message<< std::endl;
  if (!channel_->isWriteing() && outputBuffer_.getReadableBytes() == 0) {
    std::cout << "writeing" << std::endl;
    nwrite = ::write(channel_->getFd(), message.c_str(), message.size());
    if (nwrite >= 0) {
      if (boost::implicit_cast<size_t>(nwrite) < message.size()) {
        std::cout << "More Datas are going to be Writen" << std::endl;
      }
    } else {
      nwrite = 0;
      if (errno != EWOULDBLOCK) {
        std::cout << "TcpConnection::sendInLoop: " << std::endl;
      }
    }
  }
  assert(nwrite >= 0);
  if (boost::implicit_cast<size_t>(nwrite) < message.size()) {
    outputBuffer_.Append(message.c_str() + nwrite, message.size() - nwrite);
    if (!channel_->isWriteing()) channel_->enableWriteing();
  }
}
void TcpConnection::shutdown() {
  if (state_ == Connected) {
    setState(Disconnecting);
    loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, this));
  }
}
void TcpConnection::shutdownInLoop() {
  if (!channel_->isWriteing()) socket_->shutdownWrite();
}
void TcpConnection::connectEstablished() {
  // assert(state_ == Connecting);
  setState(Connected);
  channel_->tie(shared_from_this());
  channel_->enableReading();
  connectionCallBack_(shared_from_this());
}
