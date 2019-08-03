#include "Server.h"
#include <functional>
using namespace std::placeholders;
Server::Server(EventLoop* loop,
                const Address &listenAddr,
                const string& name) 
        :server_(loop,listenAddr,name)
{
    server_.setConnectionCallBack(boost::bind(&Server::onConnection,this,_1));
    server_.setMessageCallBack(boost::bind(&Server::onMessage,this,_1,_2));
}
void Server::onConnection (const TcpConnectionPtr& conn) {
    if(conn->isConnected())
    std::cout << "New Connection " << "[name: " << conn->getName() << std::endl; 
}
void Server::onMessage (const TcpConnectionPtr& conn,
                        Buffer *buf) 
{
    ;
}

void Server::onRequest (const TcpConnectionPtr&,const webRequest&) {
    ;
}

