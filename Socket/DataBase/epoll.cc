#include "epoll.h"

void epoll::Epoll::EpollCreate() {
   
    epfd_ = epoll_create(1024);
    
    assert(epfd_ != -1);

}

void epoll::Epoll::EpollAddEvent() {

    struct epoll_event ee = {0};
    int op = (*loop_.events)[fd_].mask == DataStructure::NoEvent ? 
            EPOLL_CTL_MOD : EPOLL_CTL_MOD;
    ee.events = 0;
    mask_ |= (*loop_.events)[fd_].mask;

    if(mask_ & DataStructure::ReadEvent) ee.events |= EPOLLIN;
    if(mask_ & DataStructure::WriteEvent) ee.events |= EPOLLOUT;
    ee.data.fd = fd_;
    assert(epoll_ctl(epfd_,op,fd_,&ee) != -1);            
}

void epoll::Epoll::EpollDelEvent() {
    struct epoll_event ee = {0};
    int mask = (*loop_.events)[fd_].mask & (~mask_);

    ee.events = 0;
    if(mask & DataStructure::ReadEvent) ee.events |= EPOLLIN;
    if(mask & DataStructure::WriteEvent) ee.events |= EPOLLOUT;
    ee.data.fd = fd_;
    if(mask != DataStructure::NoEvent)
        epoll_ctl(epfd_,EPOLL_CTL_MOD,fd_,&ee);
    else 
        epoll_ctl(epfd_,EPOLL_CTL_DEL,fd_,&ee);
}

int epoll::Epoll::EpollWaitEvent() {
    std::vector <struct epoll_event> tmp = *events_;
    int ret = epoll_wait(epfd_,&*tmp.begin(),loop_.setsize,-1);
    if(ret > 0) {
        for(int i = 0;i < ret; i++) {
            int mask = 0;
            struct epoll_event &eventguard = (*events_)[i];
            if(eventguard.events & EPOLLIN) mask |= DataStructure::ReadEvent;
            if(eventguard.events & EPOLLOUT) mask |= DataStructure::WriteEvent;
            if(eventguard.events & EPOLLERR) mask |= DataStructure::WriteEvent;
            if(eventguard.events & EPOLLHUP) mask |= DataStructure::WriteEvent;
            (*loop_.fired)[i].fd = eventguard.data.fd;
            (*loop_.fired)[i].mask = mask; 
        }
    }
    assert(ret > 0);
    return ret;
}