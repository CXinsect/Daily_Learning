#ifndef _PERSISTENCE_H_
#define _PERSISTENCE_H_
#include "model.h"
#include "dataBase.cc"

class Persistence {
    public:
        Persistence() {}
        Persistence(std::shared_ptr<DataBase>& ptr) : database_(ptr) { }
        static void parentHandle(int sig);
        void childHandle(int sig);
        void rdbSave();
        void CheckStorageConditions();
    private:
         long long  getTimestamp() {
            struct timeval tv;
            assert(gettimeofday(&tv,NULL) != -1);
            return tv.tv_sec;
        }
    public:
        typedef std::map<std::pair<std::string, long long>,std::string>::iterator StringIterator_;
        typedef std::map<std::pair<std::string,long long>, std::multimap<std::string, std::string>>::iterator HashIterator_;
        typedef std::map<std::pair<std::string, long long>, std::list<std::string>>::iterator ListIteator_; 
    public:
        //Rdb的数据结构
        const std::string Redis = "REDIS";
        const std::string Version = "0004";
        const std::string Database = "DATABASE";
        const std::string ExpireTime = "EXPIRETIME";
        const int Eof = 377;
        const short RedisRdbTypeString = 0;
        const short RedisRdbTypeHash = 1;
        const short RedisRdbTypeList = 2;
        long CheckSum;
    private:
        pid_t par_pid_;
        pid_t son_pid_;
        bool sigStop_ = false;
        std::string rdb_;
        long long dirty_;
        long long lastsave_ = 0;
        //记录成员大小
        long ListSize_;
        long HashSize_;
        long  databaseNum_;
        std::shared_ptr<class DataBase> database_;
        //rdb的结构
        std::string FixedStructure = Redis+Version;
        const char k = '#';
        const char k1 = '@';
        //hash key
        const char v = '$';
};
#endif