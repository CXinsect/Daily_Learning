#ifndef _EPOLL_H_
#define _EPOLL_H_
#include <sys/epoll.h>
#include <memory>
#include <string>
#include <vector>
#include <assert.h>
#include "model.h"

namespace epoll {
    class Epoll {
        public:
            Epoll(int epfd, std::shared_ptr<DataStructure::EventLoop>loop, struct epoll_event &events) : epfd_(epfd),
                loop_(loop)
{
    events_->push_back(events);
}
        void EpollCreate();
        
        void EpollAddEvent(int fd,int mask);
        
        void EpollDelEvent(int fd,int mask);
        
        int EpollWaitEvent(struct timeval *timeout);
        
        std::string getMultiplexingName() { return "Epoll"; }
        
    private:
        int epfd_;
       
        std::shared_ptr<DataStructure::EventLoop> loop_;
        
        std::shared_ptr <std::vector <struct epoll_event>> events_;
    };
}

#endif
