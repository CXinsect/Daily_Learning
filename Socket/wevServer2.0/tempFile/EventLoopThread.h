#include "Mutex.h"
#include "EventLoop.h"
#include "currentThread.h"

#include <iostream>
#include <thread>
#include <unistd.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace _Mutex;
// using namespace currentThread;

typedef boost::function<void(EventLoop*)> threadCallBack;


namespace _eventLoopThread {
    
    pid_t getTid();
    

    struct ThreadData {
        pid_t* tid_;
        threadCallBack callback_;
        ThreadData(pid_t* tid,threadCallBack cb);
    };
    void* threadFunc(void*);
    struct Thread {
       pid_t tid_;
       threadCallBack callback_;
       pthread_t pid_;
       void Start();
       void Join();
       Thread(threadCallBack  cb);
   };

    class eventLoopThread {
        public:
            eventLoopThread(threadCallBack const& cb = threadCallBack());
            ~eventLoopThread();
            EventLoop* startLoop();
        private:
            void threadFunc();
            
        private:
            EventLoop* loop_;
            bool exit_;
            Thread thread_;
            Mutex mutex_;
            Cond cond_;
            threadCallBack tcallBack_;
            pid_t tid_;
    };
  
}