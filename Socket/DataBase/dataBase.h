#include "model.h"

class DataBase {
    
    public:
        DataBase(int db_num): db_num_(db_num) { }
    public:
        const long long DefaultTime = -2038;
        void addKeySpace(int type, int encoding, const std::string &key,const std::string &value,long long expiresTime);
        void delKeySpace(int type, const std::string &key);
        std::string getKeySpace(int type, const std::string &key);
        void setKeySpaceExpiresTime(long long expiresTime) { 
            if(expiresTime != DefaultTime)
                expiresTime_ = expiresTime + getTimestamp(); 
        };
        long long getKeySpaceExpiresTime(int type,const std::string &key);
        bool judgeKeySpaceExpiresTime(int type,const std::string &key);
        void deleteKeySpaceExpireTime(int type, const std::string &key);
        long long RemainingSurvivalTime(int type,const std::string &key);
    private:
        long long  getTimestamp() {
            struct timeval tv;
            assert(gettimeofday(&tv,NULL) != -1);
            return tv.tv_sec;
        }
        
    inline std::map <std::pair<std::string, long long>, std::string>::iterator FindString(const std::string &key);
    inline std::map <std::pair<std::string, long long>, std::map<std::string, std::string>>::iterator FindHash(const std::string &key);
    inline std::map<std::pair<std::string, long long>, std::list<std::string>>::iterator FindList(const std::string &);

    private:
        int size = -1;
        int type_;
        int encoding_;
        int db_num_;
        std::string key_;
        std::string value_;
        long long expiresTime_ = DefaultTime;
        //键空间中的实际对象
        std::map <std::pair<std::string, long long>, std::string> String_;
        std::map <std::pair<std::string, long long>, std::map<std::string, std::string>> Hash_;
        std::map <std::pair<std::string, long long>, std::list<std::string>> List_;
};