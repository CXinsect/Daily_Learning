#ifndef _HANDLEEVENT_H_
#define _HANDLEEVENT_H_
#include "epoll.h"
#include "model.h"

class Handle {
    public:
        Handle (DataStructure::EventLoop loop, int fd, int mask, 
                DataStructure::FunctionCallBack callback,const std::string &data):
                loop_(loop),
                fd_(fd),
                mask_(mask),
                callback_(callback),
                clientdata_(data),
                epoll_(new epoll::Epoll(loop,fd,mask)),
                flags_(DataStructure::FileEvents) { }
    public: 
        void createEvent ();
        void delEvent();
        int  getEvent();
        int  handleEvent();
        void loopCircle();
        void stopLoop() { loop_.stoped = true; }

    private:
        DataStructure::EventLoop loop_;
        int fd_;
        int mask_;
        DataStructure::FunctionCallBack callback_;
        std::string clientdata_;
        std::shared_ptr <epoll::Epoll> epoll_;
        int flags_;
};
#endif