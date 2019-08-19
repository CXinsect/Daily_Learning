#ifndef _NET_WORKING_H_
#define _NET_WORKING_H_
#include "model.h"
#include "Buffer.h"
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
        int confd_;
        int mask_;
        std::string clientdata_;
        std::string name_;
        std::vector <RedisDataBase::_redisDb> db_;

        Buffer inputBuffer_;
        Buffer outputBuffer_;

        int argc_;
        std::vector <std::vector <RedisDataBase::_redisDb>> argv_;
        std::vector <DataStructure::redisCommand> cmd_;
        std::vector <clientStatus::_list> reply_;  //需要向客户端回复的对象
        
};

#endif