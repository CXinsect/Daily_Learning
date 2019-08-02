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
                        eventHanding_(false),
                        poller_(Poller::newDefaultPoller(this)) 
                        {};
        ~EventLoop();
        void updateChannel(Channel * channel);
        void removeChannel(Channel * channel);
        void runInLoop(const CallBack & cb);
        // bool isInLoopThread();
        void loop();
        void quit();
        // static int upFd;
                
    private:
        bool looping_;
        bool quit_;
        bool eventHanding_;
        std::shared_ptr <Poller> poller_;
        // Poller  poller_;
        typedef std::vector<Channel *> ChannelList;
        ChannelList activeChannels_;
        //处理活动的线程
        // int upFd_;
        std::unique_ptr <Channel> upchannel_;
        void handleRead();
};
#endif