#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include "currentThread.h"
#include "Universal_head.h"
#include "Channel.h"
#include "Epoll.h"
class Channel;
// class Poller;

extern __thread EventLoop* loopInThisThread;

class EventLoop {
    public:
        EventLoop() : looping_(false),
                        quit_(false),
                        eventHanding_(false),
                        // poller_(Poller::newDefaultPoller(this)) 
                        poller_(new _Poller::Poller()),
                        threadId_(currentThread::tid())
{
    if(!loopInThisThread) 
        loopInThisThread = this;
}
        ~EventLoop() {
            assert(!looping_);
            loopInThisThread = nullptr;
        }
        typedef std::function <void()> lifeCycle;
        void updateChannel(Channel * channel);
        void removeChannel(Channel * channel);
        void runInLoop(const CallBack & cb);
        bool isInLoopThread() { return threadId_ == currentThread::tid(); }
        void assertInLoopThread();
        void loop();
        void quit();
        void queueLoop(lifeCycle cb);
        // static int upFd;
                
    private:
        bool looping_;
        bool quit_;
        bool eventHanding_;
        const pid_t threadId_;
        
        typedef std::vector<Channel *> ChannelList;
        ChannelList activeChannels_;
       
        std::unique_ptr <Channel> upchannel_;
        std::vector <lifeCycle> pendingEvent_;
        std::shared_ptr <_Poller::Poller> poller_;
        void handleRead();
        void abortNotInLoopThread();

        static EventLoop* getEventLoopOfThread();
};
#endif