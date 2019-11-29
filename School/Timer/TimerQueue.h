#include "./muduoSupported/TimeStamp.h"
#include "./muduoSupported/Timer.h"
#include "./muduoSupported/TimerId.h"
#include "EventLoop.h"
#include "Channel.h"

#include <iostream>
#include <set>
#include <vector>
#include <memory>
using namespace muduo;
using namespace std;

namespace _TimerQueue {

    typedef pair<Timestamp,std::unique_ptr<Timer>> Entry;
    typedef set<Entry> timerList;
    typedef unique_ptr<Timer> timerPtr;
    class TimerQueue {
        public:
            explicit TimerQueue(EventLoop* loop);
            ~TimerQueue();
            void handleRead();
            TimerId addTimer(TimerCallback const& cb,
                                        Timestamp when,
                                        double interval);
            void addTimerInLoop(unique_ptr<Timer> timer);
            void delTimer();
        private:
            vector<Entry> getExpired(Timestamp now);
            void reset(vector<Entry>& expired,Timestamp now);
            bool insert(timerPtr timerptr);

            EventLoop* loop_;
            int const timerfd_;
            Channel timerChannel_;
            timerList timers_;  


    };
}