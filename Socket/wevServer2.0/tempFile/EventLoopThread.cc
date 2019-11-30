#include "EventLoopThread.h"

using namespace _eventLoopThread;

pid_t _eventLoopThread::getTid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void currentThread::cacheTid() {
    if(t_cachedTid == 0) {
        t_cachedTid = getTid();
    }
}
Thread::Thread(threadCallBack cb)
    :tid_(0),
    callback_(std::move(cb))
{
}
ThreadData::ThreadData(pid_t* tid,threadCallBack callback)
    :tid_(tid),
    callback_(std::move(callback))
{
}
void Thread::Start() {
    ThreadData* data = new ThreadData(&tid_,callback_);
    pthread_create(&pid_,NULL,threadFunc,data);
}

eventLoopThread::eventLoopThread(threadCallBack const& cb) 
    :loop_(0),
    exit_(false),
    mutex_(),
    cond_(mutex_),
    tcallBack_(cb),
    thread_(boost::bind(&eventLoopThread::threadFunc,this,_1))
{
}

eventLoopThread::~eventLoopThread() {
    exit_ = true;
    if(!loop_) {
        loop_->quit();
        thread_.Join();
    }
}

EventLoop* eventLoopThread::startLoop(void) {
    
    {   //减少临界区的长度
        MutexGuard lock(mutex_);
        while(loop_ == nullptr) {
            cond_.wait();
        }
        
    }
    return loop_;
}
void eventLoopThread::threadFunc() {

    EventLoop loop;
    {
        MutexGuard lock(mutex_);
        loop_ = &loop;
        cond_.signal();
    }
    loop.loop();
}