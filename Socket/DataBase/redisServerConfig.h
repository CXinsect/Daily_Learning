#ifndef _REDIS_CONFIG_H_
#define _REDIS_CONFIG_H_

#include <iostream>
#include "model.h"

class redisServerConfig {
    public:
    private:
        std::string configure_;
        std::string executable_;
        std::string exec_argv_;
        std::vector <RedisDataBase::redisDb> db_;
        std::vector <RedisDataBase::dict> commandTable_;
        DataStructure::EventLoop loop_;

        int port_;
        std::string bindAddr_;
        int bindAddrCount_;
        std::vector <int> sockfd_;

};
#endif