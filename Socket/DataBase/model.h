#include <memory>
#include <string>
#include <vector>
#include <assert.h>
#include <functional>
#include <list>
namespace DataStructure {

    typedef int (*ReadCallBack) (struct _EventLoop &eventloop, int fd, std::string clientDate, int mask);

    typedef int (*WriteCallBack) (struct _EventLoop &eventloop, int fd, std::string clientDate, int mask);

    typedef struct _SocketEvents {

        int mask;

        ReadCallBack readcallback_;
        
        WriteCallBack writecallback_;
        
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

    const int NoEvent = 0;
    const int ReadEvent = 1;
    const int WriteEvent = 2;

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

    typedef struct redisDb {
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