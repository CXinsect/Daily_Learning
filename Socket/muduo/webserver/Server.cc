#include "Server.h"
#include <iostream>
#include <functional>
const std::_Placeholder<1> std::placeholders::_1;
const std::_Placeholder<2> std::placeholders::_2;
Server::Server(EventLoop* loop,
                const Address &listenAddr,
                const string& name) 
        :server_(loop,listenAddr,name)
{
    server_.setConnectionCallBack(boost::bind(&Server::onConnection,this, _1));
    server_.setMessageCallBack(boost::bind(&Server::onMessage,this,_1,_2));
}
void Server::onConnection (const TcpConnectionPtr& conn) {
    if(conn->isConnected()) {
        std::cout << "New Connection " << "[name: " << conn->getName() << std::endl; 
        std::shared_ptr<webRequest> ptr_ (new webRequest);
        conn->setRequest(ptr_);
    }
        
}
void Server::onMessage (const TcpConnectionPtr& conn,
                        Buffer *buf) 
{
    std::shared_ptr<webRequest>request_ = conn->getRequest();
    request_->setBuffer(*buf);
    std::cout << "bufer::onMessage " << buf->retrieveAllAsString() << std::endl;
    disCription::HttpCode ret = request_->eventProcess();
    onRequest(conn,ret);
}

void Server::onRequest (const TcpConnectionPtr& conn,disCription::HttpCode status) {
    Buffer *buffer_ = new Buffer;
    webResponse response_;
    response_.setHttpCodeStatus(status);
    bool flags = response_.fileResponseAssembly(buffer_);
    if(!flags)
        conn->handClose();
    // std::cout << "Send Data: " << buffer_->retrieveAllAsString() << std::endl;
    conn->send(buffer_);
}

