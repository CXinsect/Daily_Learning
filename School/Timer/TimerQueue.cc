#include "TimerQueue.h"

using namespace muduo;
using namespace _TimerQueue;
using namespace std;


TimerId TimerQueue::addTimer(TimerCallback const& cb,
                                          Timestamp when,
                                          double interval) {
    unique_ptr<Timer> timer(new Timer(cb,when,interval));
    loop_ ->runInLoop(
        boost::bind(&TimerQueue::addTimerInLoop,this,timer)
    );
    return TimerId(timer);
}

void TimerQueue::addTimerInLoop(unique_ptr<Timer> timer) {
    loop_->assertInLoopThread();
    
}
vector< Entry> TimerQueue::getExpired(Timestamp now) {
    vector<Entry> expire;
    Timer* tmp = reinterpret_cast<Timer*>(UINTPTR_MAX);
    Entry sentry = make_pair(now, unique_ptr<Timer>(tmp));
    timerList::iterator it = timers_.lower_bound(sentry);
    assert(it == timers_.end() || now < it->first);
    copy(timers_.begin(),it,back_inserter(sentry));
    timers_.erase(timers_.begin(),it);
    return expire;
}
