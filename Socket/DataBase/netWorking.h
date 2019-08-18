#ifndef _NET_WORKING_H_
#define _NET_WORKING_H_
#include "model.h"
class NetWorking {
    public:
        NetWorking(DataStructure::EventLoop &loop, int fd, int mask, const std::string clientdata):
                    loop_(loop),
                    fd_(fd),
                    mask_(mask),
                    clientdata_(clientdata) { }
    public:
        void acceptHandler();
        
    private:
        DataStructure::EventLoop loop_;
        int fd_;
        int mask_;
        std::string clientdata_;
};

#endif