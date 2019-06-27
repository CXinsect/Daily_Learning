#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <string>
using  namespace std::placeholders;

class Server {
    public:
        Server (muduo::net::EventLoop *loop,
                const muduo::net::InetAddress &lfd);
        void start () {
            server_.start();
        }
    private:
        void onConnection(const muduo::net::TcpConnectionPtr &conn);
        void onMessage (const muduo::net::TcpConnectionPtr & conn,
                        muduo::net::Buffer *buf,
                        muduo::Timestamp time);
        muduo::net::EventLoop * loop_;
        muduo::net::TcpServer server_;
};
Server::Server(muduo::net::EventLoop *loop,
                const muduo::net::InetAddress &lfd)
        :loop_(loop),
        server_(loop,lfd,"discard")
{
    server_.setMessageCallback(bind(&Server::onMessage,this,_1,_2,_3));
}
void Server::onMessage (const muduo::net::TcpConnectionPtr &conn,
                        muduo::net::Buffer *buf,
                        muduo::Timestamp time) 
{
    muduo::string str(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << "discards " << str.size()
             << "bytes received at " << time.toString();
}
int main (int argc,char * argv[]) {
    LOG_INFO << "pid = " << getpid() ;
    muduo::net::EventLoop loop;
    muduo::net::InetAddress lfd (8888);
    Server server(&loop,lfd);
    server.start();
    loop.loop();
    return 0;
}
