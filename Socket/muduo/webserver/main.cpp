#include "EventLoop.cc"
#include "Address.cc"
#include "SocketOpts.cc"
#include "Acceptor.cc"
using namespace sockets;
void newConnection(int sockfd,const Address &perrAddr) {
   std::cout << "NewConnection" << perrAddr.toIpPort().c_str() << std::endl;
    ::write(sockfd,"hello world\n",13);
    sockets::close(sockfd);
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