#ifndef _ACCEPT_H_
#define _ACCEPT_H_
#include "model.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.cc"
// #include "SocketOpts.cc"
// #include "InetAddress.h"
class InetAddr;

class Accept {
    public:
        typedef std::function<void(int sockfd,
                                       const Address &)> ConnectionCallBack;
        Accept(EventLoop* loop,const Address&);
        void setConnectionCallBack(const ConnectionCallBack &cb) {
            connectioncallback_ = cb;
        }
        bool listening() { return listening_; }
        void listen();
    private:
        void handleRead();
        EventLoop* loop_;
        Socket acceptSocket_;
        Channel acceptChannel_;
        ConnectionCallBack connectioncallback_;
        bool listening_;
};
#endif