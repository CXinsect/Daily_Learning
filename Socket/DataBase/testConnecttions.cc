#include <iostream>
// #include "InetAddress.cc"
#include "EventLoop.cc"
#include "Channel.cc"
#include "Accept.cc"
void connection(int sockfd, const Address& peerAddr) {
    std::cout << "NewConnection: " << peerAddr.toIpPort() << std::endl;
    ::write(sockfd,"hello,Linux",11);
    ::close(sockfd);
}

int main() {
    Address listenaddr (8888);
    EventLoop loop;
    Accept acceptor(&loop,listenaddr);
    acceptor.setNewConnectionCallBack(connection);
    acceptor.listen();
    loop.loop();
}