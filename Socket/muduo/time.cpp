#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <string>
using namespace std;
using namespace std::placeholders;
class Server {
 public:
        Server(muduo::net::EventLoop *loop,
                const muduo::net::InetAddress &lfd);
        void start(void) {
            server_.start();
        }
    private:
        void onConnection (const muduo::net::TcpConnectionPtr &conn);
        muduo::net::EventLoop * loop_;
        muduo::net::TcpServer server_;
};
Server::Server(muduo::net::EventLoop *loop,
                    const muduo::net::InetAddress &lfd)
                    : loop_(loop),
                    server_(loop,lfd,"time") {
    server_.setConnectionCallback(bind(&Server::onConnection,this,_1));
}
void Server::onConnection (const muduo::net::TcpConnectionPtr &conn)
{
    LOG_INFO << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort();
    if(conn->connected()) {
        time_t now = ::time(NULL);
        int32_t t = muduo::net::sockets::hostToNetwork32(static_cast <int32_t> (now));
        conn->send(&t,sizeof(t));
        conn->shutdown();
    }
}
int main (void) {
    LOG_INFO << "PID: " << getpid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress lfd (8888);
    Server server(&loop,lfd);
    server.start();
    loop.loop();
    return 0;
}