#ifndef ADDRESS__H__
#define ADDRESS__H__
#include <sys/socket.h>

#include <boost/noncopyable.hpp>
#include <boost/implicit_cast.hpp>
#include <string>
class Address {
    public:
        explicit Address (short port = 0,bool loopbackOnly = false);
        Address (std::string &ip,short port);
        explicit Address (struct sockaddr_in & addr) : 
                        addr_(addr) {}
        sa_family_t family () const { return addr_.sin_family; }
        struct sockaddr* getSockAddr () const {
            struct sockaddr * addr4 = static_cast <struct sockaddr*>(boost::implicit_cast<void*>(&addr_));
            return addr4;
        }
        std::string toIp () const;
        std::string toIpPort() const;
        short toPort() const;

    private:
        struct sockaddr_in addr_;
};
#endif