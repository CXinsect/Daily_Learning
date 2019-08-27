#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "model.h"
// #include "EventLoop.h"
class EventLoop;
class Channel {
    public:
        Channel(EventLoop*loop, const int &sockfd):
                        loop_(loop),
                        sockfd_(sockfd),
                        events_(0),
                        revents_(0),
                        index_(-1) {}
        typedef std::function <void()>  EventCallBack;
        void  handleEvent();
        void  setReadCallback(const EventCallBack &cb)
        { readcallback_ = cb; }
        void setWriteCallback(const EventCallBack &cb)
        { writecallback_ = cb; }
        int getSockfd() { return sockfd_; }
        void setRevents(int revt) { revents_ = revt; }
        void setIndex(int index) { index_ = index; }
        int getEvents() { return events_; }
        int getIndex() { return index_; }
        bool isNoneEvent() { return events_ == NoneEvent_; }

        void enableReading() { events_ |= ReadEvent_; update();}
        void enableWriting() { events_ |= WriteEvent_; update(); }
        void disableWriting() { events_ &= ~WriteEvent_; update(); }
        void disableAll() { revents_ &= NoneEvent_; update(); }
    private:
        void update();
        static const int NoneEvent_;
        static const int ReadEvent_;
        static const int WriteEvent_;
        EventLoop *loop_;
        int sockfd_;
        int revents_;
        int events_;
        int index_;
        EventCallBack readcallback_;
        EventCallBack writecallback_;
};
const int Channel::NoneEvent_ = 0;
const int Channel::ReadEvent_ = EPOLLIN | EPOLLPRI;
const int Channel::WriteEvent_ = EPOLLOUT;
#endif