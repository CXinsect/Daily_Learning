#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_
#include <functional>
#include "Socket.h"
#include "Address.h"
class Acceptor {
    public:
        typedef std::function <void (int sockfd,const Address &)> NewConnectionback;
    Acceptor()
}
#endif