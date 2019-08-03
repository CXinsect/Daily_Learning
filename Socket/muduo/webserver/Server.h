#ifndef _SERVER_H_
#define _SERVER_H_
#include <iostream>
#include "Universal_head.h"
#include "EventLoop.h"
#include "Address.h"
#include "TcpServer.h"
#include "TcpConnection.h"
class Server {
    public:
        typedef std::function<void(const webRequest&,webRespnse*)>webCallBack;

        Server(EventLoop * loop,
                const Address &addr,
                const string &name);
        
        void setCallBack(const webCallBack &cb) {
            webcallback_ = cb;
        }
        void start() { server_.start(); }
        private:
            void onConnection(const TcpConnectionPtr &conn);
            void onMessage(const TcpConnectionPtr& conn,
                            Buffer * buf);
            void onRequest(const TcpConnectionPtr&,const webRequest&);
            TcpServer server_;
            webCallBack webcallback_; 
};

#endif