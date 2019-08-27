#include "Channel.h"


void Channel::update() {
    loop_->updateChannel(this);
}
void Channel::handleEvent() {
    if(revents_ & (EPOLLIN | EPOLLPRI) )
        if(readcallback_) readcallback_();
    if(revents_ & EPOLLOUT)
        if(writecallback_) writecallback_();
}