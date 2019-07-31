#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include "Universal_head.h"
 #include "Poller.h"
#include "Channel.h"
class Channel;
// class Poller;
class EventLoop {
    public:
        EventLoop() : looping_(false),
                        quit_(false),
                        poller_(Poller::newDefaultPoller(this)) {};
        ~EventLoop() {};
        void updateChannel(Channel * channel);
        void loop();
        void quit();
    private:
        bool looping_;
        bool quit_;
        std::shared_ptr <Poller> poller_;
        // Poller  poller_;
        typedef std::vector<Channel *> ChannelList;
        ChannelList activeChannels_;
};
#endif