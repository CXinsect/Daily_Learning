#include <iostream>
#include <memory>
#include <thread>
#include <boost/implicit_cast>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class portScan {
    public:
        portScan(unsigned short s_port_,unsigned short d_port_,
                const struct in_addr ip_):s_port(s_port_),d_port(d_port_),ip(ip_) {}
        bool doScan(struct sockaddr_in *addr);
    private:
        unsigned short s_port;
        unsigned short d_port;
        struct in_addr ip;
};
bool portScan::doScan(struct sockaddr_in *addr) {
    int confd;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0) {
        std::cout << "socket err" << std::endl;
        exit(0);
    }
    struct sockaddr * addr4 = static_cast<struct sockaddr*>(boost::implicit_cast<void*>(addr));
    int ret = connect(sockfd,static_cast<struct sockaddr*>(addr),sizeof(*addr));
    if(ret == 0) {
        std::cout << ntohs(addr->sin_port) << " has started " <<
        "in ip " << inet_ntoa(addr->sin_addr) << std::endl;
        close(confd);
        return true;
    }
    else if(errno == ECONNREFUSED) {
        std::cout << "port has stoped" << std::endl;
        close(confd);
        return false;   
    }
    else {
        std::cout << "Other Err" << std::endl;
        return false;
    }
    return false;
}

int main (int argc,char *argv[]) {
    unsigned short maxport;
    unsigned short souport;
    int seg_len;
    if(argc < 7) {
        std::cout << "[-m][maxport][-a][address][-n][thread]"<<std::endl;
        exit(1);
    }
    for(int i = 1;i < 7;i++) {
        if(strcmp(argv[i],"-m") == 0) {
            maxport = atoi(argv[i+1]);
            continue;
        }
    }
    return 0;
}



