#include "Server.cc"

int main (void) {
    EventLoop loop;
    Address listenaddr(8888);
    Server server(&loop,listenaddr);
    loop.loop();
}