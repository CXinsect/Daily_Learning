#include "handleEvent.h"

void Handle::createEvent() {
    assert(fd_ <= loop_.setsize);
    DataStructure::SocketEvents &fe = (*loop_.events)[fd_];
    epoll_->EpollAddEvent();

    fe.mask |= mask_;
    if(mask_ & DataStructure::ReadEvent) fe.readcallback_ = callback_;
    if(mask_ & DataStructure::WriteEvent) fe.writecallback_ = callback_;
    fe.clientDate = clientdata_;
}

void Handle::delEvent () {
    assert(fd_ < loop_.setsize);
    DataStructure::SocketEvents &fe = (*loop_.events)[fd_];
    assert(fe.mask == DataStructure::NoEvent);
    epoll_->EpollDelEvent();
    fe.mask = fe.mask & (~mask_);
}

int Handle::getEvent() {
    assert(fd_ < loop_.setsize);
    DataStructure::SocketEvents &fe = (*loop_.events)[fd_];
    return fe.mask;
}

int Handle::handleEvent() {
    assert(!(flags_ & DataStructure::FileEvents) && !(flags_ & DataStructure::TimeEvents));
    int numevents = epoll_->EpollWaitEvent();
    int processed = 0;
    for(int i = 0;i < numevents; i++) {
        DataStructure::SocketEvents &fe = (*loop_.events)[(*loop_.fired)[i].fd];
        int mask = (*loop_.fired)[i].mask;
        int fd   = (*loop_.fired)[i].fd;
        int fired = 0;
        if(fe.mask &mask & DataStructure::ReadEvent) {
            fe.readcallback_(loop_,fd,fe.clientDate,mask);
            fired++;
        }
        if(fe.mask & mask & DataStructure::WriteEvent) {
            fe.writecallback_(loop_,fd,fe.clientDate,mask);
            fired++;
        }

        processed++;
    }
    return processed;
}

void Handle::loopCircle() {
    loop_.stoped = false;
    while(!loop_.stoped) {
        handleEvent();
    }
}