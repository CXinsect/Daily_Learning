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
            Epoll(DataStructure::EventLoop &loop,int fd, int mask) : loop_(loop),
                                                                     fd_(fd),
                                                                     mask_(mask) {}
        void EpollCreate();
        
        void EpollAddEvent();
        
        void EpollDelEvent();
        
        int EpollWaitEvent();
        
        std::string getMultiplexingName() { return "Epoll"; }
        
    private:

        int fd_;

        int mask_;

        int epfd_;
       
        DataStructure::EventLoop loop_;
        
        std::shared_ptr <std::vector <struct epoll_event>> events_;
    };
}

#endif
