#ifndef  _TCP_SERVER_H_
#define  _TCP_SERVER_H_
#include "EventLoop.h"
#include "Address.h"
#include "Acceptor.h"
#include "Universal_head.h"
class TcpConnection;

class TcpServer {
    public:
    ~TcpServer();
    void start();
    void setConnectionCallBack(const ConnectionCallBack& cb) {
        connectionBack_ = cb;
    }
    void setMessageCallBack(const MessageCallBack& cb) {
        messageBack_ = cb;
    }
    void SetCloseCallBack(const CloseCallBack& cb) {
        closeCallBack_ = cb;
    } 
    private:
        void removeConnection(const TcpConnectionPtr& conn);
        void newConnection(int sockfd,const Address &peerAddr);
        typedef std::map<std::string,TcpConnectionPtr> ConnectionMap;
        EventLoop *loop_;
        const std::string name_;
        boost::scoped_ptr <Acceptor> acceptor;
        ConnectionCallBack connectionBack_;
        MessageCallBack messageBack_;
        CloseCallBack closeCallBack_;
        bool started_;
        int nextConfd_;
        ConnectionMap connection_;
};

#endif