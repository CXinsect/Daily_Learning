#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/Endian.h>
#include <muduo/net/InetAddress.h>
#include <string>
#include <utility>
using namespace std;
using namespace std::placeholders;

class Client {
    public:
    Client (muduo::net::EventLoop *loop,
            const muduo::net::InetAddress &srv);
    inline void connect (void) {
            client_.connect();
        }
    private:
        void onMessage (const muduo::net::TcpConnectionPtr &conn,
                        muduo::net::Buffer *buf,
                        muduo::Timestamp time);
        void onConnection(const muduo::net::TcpConnectionPtr &conn);
       
        muduo::net::EventLoop * loop_;
        muduo::net::TcpClient client_;
};
Client::Client(muduo::net::EventLoop *loop,
                const muduo::net::InetAddress &srv)
                :loop_(loop),client_(loop,srv,"timeclient") {
    client_.setConnectionCallback(bind(&Client::onConnection,this,_1));
    client_.setMessageCallback(bind(&Client::onMessage,this,_1,_2,_3));
}
void Client::onConnection (const muduo::net::TcpConnectionPtr &conn) {
    LOG_INFO << conn->localAddress().toIpPort() << " -> " << conn->peerAddress().toIpPort() << 
    " is " << (conn->connected() ? " kai " : " guan ");
    if(!conn->connected()) {
        loop_->quit();
    }
}
void Client::onMessage (const muduo::net::TcpConnectionPtr &conn,
                        muduo::net::Buffer *buf,
                        muduo::Timestamp time) {
    if(buf->readableBytes() >= sizeof(int32_t)) {
        const void *data = buf->peek();
        int32_t t = *static_cast <const int32_t *>(data);
        buf->retrieve(sizeof(int32_t));
        time_t time = muduo::net::sockets::networkToHost32(t);
        muduo::Timestamp ts (time * muduo::Timestamp::kMicroSecondsPerSecond);
        LOG_INFO << "Server Time: ==== " << time << ts.toFormattedString();
    }
    else {
        LOG_INFO << conn->name() << "no enough data "<<buf->readableBytes() << "at" <<
        time.toFormattedString();
    }
}
int main (int argc,char * argv[]) {
    LOG_INFO << "pid "<< getpid() << "\n";
    muduo::net::EventLoop loop;
    muduo::net::InetAddress srv (argv[1],8888);
    Client client(&loop,srv);
    client.connect();
    loop.loop();
    return 0;
}