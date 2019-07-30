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
    index_(-1) {}

void Channel::update (void) {
    loop_->updateChannel(this);
}

void Channel::handleEvent() {
    if(events_ & (POLLNVAL | POLLERR))
        if(errCallBack_) errCallBack_();
    if(events_ &(POLLIN | POLLPRI | POLLRDHUP))
        if(readCallBack_) readCallBack_();
    if(revents_ & POLLOUT)
        if(writeCallBack_) writeCallBack_();    
}