#include "Channel.h"
#include "EventLoop.h"

const int Channel::NoneEvent = 0;
const int Channel::ReadEvent = POLLIN | POLLPRI;
const int Channel::WriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0),
      index_(-1),
      eventHanding_(false),
      tied_(false) {}

void Channel::update(void) { loop_->updateChannel(this); }

void Channel::handleEvent() {
  boost::shared_ptr <void> ret;
  if (tied_) {
    ret = tie_.lock();
    if (ret) {
      eventHanding_ = true;
      if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        std::cout << "Channel::handleEvent() POLLHUP" << std::endl;
        if (closeCallBack_) closeCallBack_();
      }
      if (revents_ & (POLLNVAL | POLLERR))
        if (errCallBack_) errCallBack_();
      if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
        if (readCallBack_) readCallBack_();
<<<<<<< HEAD
      if (revents_ & POLLOUT)
        if (writeCallBack_) writeCallBack_();
=======
      if (revents_ & POLLOUT) {
        std::cout << "comming in" << std::endl;
          if (writeCallBack_) writeCallBack_();
      }
        
>>>>>>> test1
      eventHanding_ = false;
    }
  }
  else {
      eventHanding_ = true;
      if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        std::cout << "Channel::handleEvent() POLLHUP" << std::endl;
        if (closeCallBack_) closeCallBack_();
      }
      if (revents_ & (POLLNVAL | POLLERR))
        if (errCallBack_) errCallBack_();
      if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
        if (readCallBack_) readCallBack_();
      if (revents_ & POLLOUT)
        if (writeCallBack_) writeCallBack_();
      eventHanding_ = false;
  }
}
void Channel::remove(void) {
  assert(isNoneEvent());

  loop_->removeChannel(this);
}

void Channel::tie(const boost::shared_ptr<void>& obj) {
  tie_ = obj;
  tied_ = true;
}
Channel::~Channel() {
  assert(!eventHanding_);
  this->disableAll();
  this->remove();
  ::close(fd_);
}