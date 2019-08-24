#ifndef _PERSISTENCE_H_
#define _PERSISTENCE_H_
#include "model.h"
#include "dataBase.h"

class DataBase;

class Persistence {
    public:
        Persistence(std::shared_ptr<DataBase>& ptr) : database_(ptr) { }
        void parentHandle(int sig);
        void childHandle(int sig);
        void rdbSave(const std::string filename);
    public:
        typedef std::map<std::pair<std::string, long long>,std::string>::iterator StringIterator_;
        typedef std::map<std::pair<std::string,long long>, std::map<std::string, std::string>>::iterator HashIterator_;
        typedef std::map<std::pair<std::string, long long>, std::list<std::string>>::iterator ListIteator_; 
    public:
        //Rdb的数据结构
        const std::string Redis = "REDIS";
        const std::string Version = "0004";
        const std::string DataBase = "DATABASE";
        const std::string ExpireTime = "EXPIRETIME";
        const int Eof = 377;
        const short RedisRdbTypeString = 0;
        const short RedisRdbTypeHash = 1;
        const short RedisRdbTypeList = 2;
        long CheckSum;
    private:
        pid_t par_pid_;
        pid_t son_pid_;
        std::string rdb_;
        long long dirty_;
        time_t lastsave_;
        //记录成员大小
        long ListSize_;
        long HashSize_;
        long  databaseNum_;
        std::shared_ptr<class DataBase> database_;
        //rdb的结构
        std::string FixedStructure = Redis+Version;
};
#endif