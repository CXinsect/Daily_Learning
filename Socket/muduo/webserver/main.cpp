#include "EventLoop.cc"
#include "Address.cc"
#include "SocketOpts.cc"
#include "Acceptor.cc"
using namespace sockets;
void onConnection(TcpConnectionPtr &conn) {
    if(conn->connected)
}
int main (void) {
    printf("main pid: %d\n",getpid());
    EventLoop loop;
    Address listenAddr(8888);
    Acceptor acceptor(&loop,listenAddr);
    acceptor.setNewConnectionCallBack(newConnection);
    acceptor.Listen();
    loop.loop();
    return 0;
}