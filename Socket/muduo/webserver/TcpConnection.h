#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_
#include "Universal_head.h"
#include "EventLoop.h"
#include "Address.h"
#include "Socket.h"
#include "Channel.h"

class TcpConnection : 
            public boost::enable_shared_from_this<TcpConnection>
{
    public:     
        TcpConnection(EventLoop *loop,const std::string name,int sockfd,
                              const Address &localAddr,const Address &peerAddr) 
                        :loop_(loop),
                        name_(name),
                        socket_(new Socket(sockfd)),
                        channel_(new Channel(loop,sockfd)),
                        localAddr_(localAddr),
                        peerAddr_(peerAddr) 
        {
            channel_->setReadCallBack(
                std::bind(&TcpConnection::handleRead,this));
        }
        void setConnectionCallBack(ConnectionCallBack &cb) {
            connectionCallBack_ = cb;
        }
        void setMessageCallBack(MessageCallBack &cb) {
            messageCallBack_ = cb;
        }
        void setCloseCallBack(const CloseCallBack &cb) {
            closeCallBack_ = cb;
        }
        const string & getName() { return name_; }
    private:
        enum State { Connecting,Connected,Disconnceted};
        EventLoop *loop_;
        std::string name_;
        State state_;
        boost::scoped_ptr<Socket> socket_;
        boost::scoped_ptr<Channel> channel_;
        Address localAddr_;
        Address peerAddr_;
        ConnectionCallBack connectionCallBack_;;
        MessageCallBack messageCallBack_;
        CloseCallBack closeCallBack_;
        // ErrCallBack errCallBack_;
        void setState(State s) { state_ = s;}
        void handleRead();
        void handWrite();
        void handClose();
};
#endif