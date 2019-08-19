#ifndef _MODEL_H_
#define _MODEL_H_
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h> /* See NOTES */
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <assert.h>
#include <functional>
#include <list>
#include <boost/implicit_cast.hpp>
namespace DataStructure {

    typedef int (*FunctionCallBack) (struct _EventLoop &eventloop, int fd, std::string clientDate, int mask);

    // typedef int (*WriteCallBack) (struct _EventLoop &eventloop, int fd, std::string clientDate, int mask);

    typedef struct _SocketEvents {

        int mask;

        FunctionCallBack readcallback_;
        
        FunctionCallBack writecallback_;
        
        std::string clientDate;

    }SocketEvents;
    
    typedef struct _FiredType {
        int mask;
        int fd;
    }FiredType;

    typedef struct _EventLoop {

        int setsize;
        
        std::shared_ptr<std::vector <SocketEvents>> events;

        std::shared_ptr<std::vector <FiredType>> fired;

        bool stoped;

        std::string clientDate;
    }EventLoop;

    typedef struct _redisObject {
        unsigned type:4;
        unsigned encoding:4;
        std::string data;
    }redisObject;
    
    typedef std::function<void()> CommandCallBack;
    typedef struct _redisCommand {
        std::string orderName;
        CommandCallBack commandcallback_;
        int arity;//参数个数
        std::string sflags; //命令属性
        int flags; //二进制命令标识
    }redisCommand;

    DataStructure::redisCommand cmdTable [] {
            {"get",getCommand,2,"r",0},
            {"set",setCommand,3,"w",0},
            {"del",delCommand,2,"w",0},
            {"bgsave",bgsaveCommand,1,"a",0}
    };
    
    const int NoEvent = 0;
    const int ReadEvent = 1;
    const int WriteEvent = 2;
    
    const int FileEvents = 3;
    const int TimeEvents = 4;

    const int Port = 8888;
    const std::string Ip = "127.0.0.1";

}

namespace RedisDataBase {
    typedef std::function <void (const void*key)> hashFunction;
    
    /*实现哈希*/
    typedef struct _dictEntry {
        void *key;
        void *val;
        struct _dictEntry *next;
    }dictEntry;

    typedef struct _dictht {
        std::vector <dictEntry> table;

        unsigned long size;

        unsigned long sizemask;

        unsigned long used;
    }dicht;

    typedef struct _dict {
        hashFunction type;
        std::string priData;
        dicht ht[2];
        long rehashindex;
    }dict;

    typedef struct _redisDb {
        std::vector <dict> dictable;
        std::vector <dict> expires;
        int id;
    }redisDb;
}
namespace clientStatus {
    typedef struct _list {
        std::list <void *> clientList;
        std::function <int (void *ptr,void *key)> match;
        unsigned long len;
    }list_;
}

#endif