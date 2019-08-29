#ifndef _SERVER_H_
#define _SERVER_H_

#include "EventLoop.cc"
#include "Channel.cc"
#include "Accept.cc"

class Server {
    public:
        Server(EventLoop *loop,const Address &peeraddr) : 
                    loop_(loop),
                    accept_(std::shared_ptr<Accept>(new Accept(loop,peeraddr)))
        { 
            accept_->setConnectionCallBack(std::bind(&Server::onConnection,this,_1));
            accept_->setMessageCallBack(std::bind(&Server::onMessage,this,_1,_2,_3));
        }
    public:
        void onConnection(const AcceptorPtr& conn);
        void onMessage(const AcceptorPtr& conn,Buffer *buf,ssize_t n);
        void commandRrequest(Buffer *buf);
        void commandReply(Buffer *buf);
    private:
        EventLoop *loop_;
        std::shared_ptr<Accept> accept_;
};
#endif