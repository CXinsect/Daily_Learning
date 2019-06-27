#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <string.h>
using namespace std::placeholders;
class Server {
    public:
        Server(muduo::net::EventLoop *loop,
                const muduo::net::InetAddress &lfd);
        void start(void) {
            server_.start();
        }
    private:
        void onMsessage (const muduo::net::TcpConnectionPtr &conn,
                        muduo::net::Buffer *buf,
                        muduo::Timestamp time);
        muduo::net::EventLoop * loop_;
        muduo::net::TcpServer server_;
};
Server::Server(muduo::net::EventLoop *loop,
                const muduo::net::InetAddress &lfd)
                :loop_(loop),server_(loop,lfd,"daytime") {
    server_.setMessageCallback(bind(&Server::onMsessage,this,_1,_2,_3));
}
void Server::onMsessage (const muduo::net::TcpConnectionPtr &conn,
                        muduo::net::Buffer *buf,
                        muduo::Timestamp time) 
{
    LOG_INFO << "Daytime: "<< conn->peerAddress().toIpPort() << " to "
             << " local " << conn->localAddress().toIpPort() << " is "
             <<(conn->connected() ? "up" : "down");
             if(conn->connected()) {
                 conn->send (muduo::Timestamp::now().toFormattedString() + "\n");
                 conn->shutdown();
             }
}
int main (void) {
    LOG_INFO << "pid" << getpid() <<"\n";
    muduo::net::EventLoop loop;
    muduo::net::InetAddress lfd (8888);
    Server server(&loop,lfd);
    server.start();
    loop.loop();
    return 0;
}