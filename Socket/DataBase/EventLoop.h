#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_
#include "model.h"

#include "Epoll.cc"

class EventLoop {
    public:
         EventLoop(): looping_(false),epoll_(new Epoll)  {
            int epd = ::epoll_create(10);
            epoll_->setEpollfd(epd);
         };
        ~EventLoop() { assert(!looping_); };
        typedef std::vector<class Channel*> ChannelList;
        bool quit_;
        // boost::scoped_ptr <class Epoll> epoll_;
        std::shared_ptr<Epoll> epoll_;
        void loop();
        void updateChannel(Channel * channel);
        void  quit() { quit_ = true; }
    private:
        bool looping_;
        ChannelList activeChannels_;
};
#endif