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
        // std::vector <RedisDataBase::dict> commandTable_;
        DataStructure::EventLoop loop_;

        int port_ = 8888;
        std::string bindAddr_ = std::string();
        int bindAddrCount_ = 0;;
        std::vector <int> sockfd_;

        bool loading_ = false;
        int loadSize_ = 0;
        long numconnections_ = 0;
        int dbnum_ = 16;
};
#endif