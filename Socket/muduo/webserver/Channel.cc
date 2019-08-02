#include "EventLoop.h"
#include "Channel.h"

const int Channel::NoneEvent = 0;
const int Channel::ReadEvent = POLLIN | POLLPRI;
const int Channel::WriteEvent = POLLOUT;

Channel::Channel (EventLoop* loop,int fd) 
    :loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(-1),
    eventHanding_(false) {}

void Channel::update (void) {
    loop_->updateChannel(this);
}

void Channel::handleEvent() {
     eventHanding_ = true;
    if((revents_ &POLLHUP) && !(revents_ &POLLIN)) {
        std::cout << "Channel::handleEvent() POLLHUP" <<std::endl;
        if(closeCallBack_) closeCallBack_();
    }
    if(revents_ & (POLLNVAL | POLLERR))
        if(errCallBack_) errCallBack_();
    if(revents_ &(POLLIN | POLLPRI | POLLRDHUP))
        if(readCallBack_) readCallBack_();
    if(revents_ & POLLOUT)
        if(writeCallBack_) writeCallBack_();
    eventHanding_ = false;    
}
void Channel::remove(void) {
    assert(isNoneEvent());
    
    loop_->removeChannel(this);
}