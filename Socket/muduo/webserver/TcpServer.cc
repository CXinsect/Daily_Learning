#include "TcpServer.h"
#include "SocketOpts.h"
#include "Address.h"
#include "TcpConnection.h"
// using namespace sockets;
// class TcpConnection;

void TcpServer::newConnection (int sockfd,const Address& peerAddr) {
    char buf[32];
    snprintf(buf,sizeof(buf),"#%d",nextConfd_);
    ++nextConfd_;
    std::string name = name_ + buf;
    std::cout << "TcpServer::New Connection: " << name_ 
              << "from " << peerAddr.toIpPort().c_str() << std::endl;
    // struct sockaddr_in addr_= sockets::getLocalAddr(sockfd);
    Address localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn (new TcpConnection(loop_,name,sockfd,localAddr,peerAddr));
    connection_[name] = conn;
    conn->setConnectionCallBack(connectionBack_);
    conn->setMessageCallBack(messageBack_);
    conn->setCloseCallBack(boost::bind(&TcpServer::removeConnection, this, _1));
    // conn->connEstablished();
}
void TcpServer::removeConnection(const TcpConnectionPtr &conn) {
    std::cout << "removeConnection: "<< name_ <<std::endl;
    connection_.erase(conn->getName());  
}