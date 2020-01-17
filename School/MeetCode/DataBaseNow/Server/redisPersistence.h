#ifndef _PERSISTENCE_H_
#define _PERSISTENCE_H_
#include "model.h"
// #include "dataBase.h"
#include "./util/status.h"
#include "threadpool.h"
#include <mutex>
#include <atomic>
#include "File.h"
#include <algorithm>
#include <unordered_map>

class DataBase;

using namespace std;
class threadStorage : public Task {
    public:
       
        threadStorage(shared_ptr<DataBase>&db,int dbNum):database_(db),dbNums_(dbNum) {
         }
       ~threadStorage() { }
        void rdbSave();
        void Run() { CheckStorageConditions(); }
        void print(); 
        void setDataBase(shared_ptr<DataBase>& db) {
            database_ = db;
        }
         static int fileId_;
    
    private:
        int incrementFileId () {
            // unique_lock<mutex> mylock(mutex_);
            ++fileId_;
            std::cout << "why" << fileId_ << endl;
            return fileId_;
        }
        int getCurFileId() { 
            // unique_lock<mutex> mylock(mutex_);            
            return fileId_; 
        }
        
        string intToString (int t) {
            string s = "";
            while(t) {
                s += '0' + t % 10;
                t /= 10;
            }
            reverse(s.begin(),s.end());
            return s;
        }
    public:
        bool CheckStorageConditions();
        // void setDataBase(DataBase &database) {}
        
    private:
         long long  getTimestamp() {
             struct timeval tv;
             assert(gettimeofday(&tv, NULL) != -1);
             return tv.tv_sec;
         }

         void setOffset(int size) { offsetend_ = size; }
    private:
        // int fileId_;
        mutex mutex_;   
        int offsetend_ = 0;
        uint64_t location_;
        shared_ptr<DataBase> database_;
        unordered_map<string,uint64_t> keyLocation_;
        unordered_map<string,string> keyPath_;
        bool hasFileFlag_ = false;
        int fd_ = -1;
        char* mptr_;
        
    public:
        string findFileEntries(string key) {
            auto it = keyPath_.find(key);
            if(it != keyPath_.end()) {
                int fd = open(it->second.c_str(),O_RDONLY,0644);
                if(fd != -1) {
                    struct stat st;
                    assert(stat(it->second.c_str(),&st) != -1);
                    char*p = static_cast<char*>(mmap(0,st.st_size,PROT_READ,MAP_SHARED,fd,0));
                
                    auto iter = keyLocation_.find(key);
                    if(iter != keyLocation_.end()) {
                        int len = iter->second >> 32;
                        int offset = iter->second & 0xFFFFFFFF;
                        
                    }    
                }
                
                else {
                    cout << "file open error[persistence]: " << __LINE__ <<endl;
                    exit(0);
                }
            }
        }

        void Close() {
            cout << "close" << endl;
            close(fd_);
        }
    
    private:
        public:
        //Rdb的数据结构
        const std::string Redis = "REDIS";
        const std::string Version = "0004";
        const std::string Database = "FE";
        const std::string ExpireTime = "FD";
        const int Eof = 377;
        const short RedisRdbTypeString = 0;
        const short RedisRdbTypeHash = 1;
        const short RedisRdbTypeList = 2;
        long CheckSum;


    private:
        std::string rdb_;
        long long dirty_;
        long long lastsave_ = 0;
        //记录成员大小
        long ListSize_;
        long HashSize_;
        long  databaseNum_;
        //rdb的结构
        std::string FixedStructure = Redis+Version;
        const char k = '#';
        const char k1 = '@';
        //hash key
        const char v = '$';
    public:
        int dbNums_;
};
class Persistence {
    public:
        Persistence() {}
        Persistence(std::shared_ptr<DataBase>& ptr,int dbNums) : database_(ptr),dbNums_(dbNums) { }
        static void parentHandle(int sig);
        void childHandle(int sig);
        void rdbSave();
        bool CheckStorageConditions();
        void setDataBase(DataBase &database) {}
    public:
        
    private:
         long long  getTimestamp() {
             struct timeval tv;
             assert(gettimeofday(&tv, NULL) != -1);
             return tv.tv_sec;
         }

    // public:
    //     typedef std::map<std::pair<std::string, long long>,std::string>::iterator StringIterator_;
    //     typedef std::map<std::pair<std::string,long long>, std::multimap<std::string, std::string>>::iterator HashIterator_;
    //     typedef std::map<std::pair<std::string, long long>, std::list<std::string>>::iterator ListIteator_; 
    public:
        //Rdb的数据结构
        const std::string Redis = "REDIS";
        const std::string Version = "0004";
        const std::string Database = "FE";
        const std::string ExpireTime = "FD";
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
    public:
        int dbNums_;
    
};
#endif