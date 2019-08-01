#include "EventLoop.cc"
#include "Address.cc"
#include "SocketOpts.cc"
#include "Acceptor.cc"
#include "TcpConnection.cc"
#include "TcpServer.cc"
// #include "Buffer.cc"
using namespace sockets;
void onConnection(const TcpConnectionPtr &conn) {
    if(conn->isConnected()) {
        std::cout << "connection: " << conn->getName() << std::endl;
        // sleep(500);
        std::string s1("hello");
        std::string s2("world");
       
        conn->send(s1);
        conn->send(s2);
        conn->shutdown();
        std::cout << "sent" << std::endl;
    }
    else 
        std::cout << "disconnected" << std::endl;
}
void onMessage(const TcpConnectionPtr& conn,
                Buffer * buf) {
    std::cout << "Recrive: " << buf->retrieveAsString(buf->getReadableBytes()).c_str() << std::endl;
}
int main (void) {
    printf("main pid: %d\n",getpid());
    EventLoop loop;
    Address listenAddr(8888);
    TcpServer server(&loop,listenAddr,"cxinsect");
    server.setConnectionCallBack(onConnection);
    server.setMessageCallBack(onMessage);
     server.start();
     loop.loop();
    return 0;
}