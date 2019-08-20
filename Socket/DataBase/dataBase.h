#include "model.h"

class DataBase {
    
    public:
        DataBase(int db_num): db_num_(db_num) { }
    public:
        void addKeySpace(int type, int encoding, const std::string &key,const std::string &value,long long ExpiresTime);
        void delKeySpace(int type, const std::string &key);
        std::string getKeySpace(int type, const std::string &key);
        void setKeySpaceExpiresTime(long long expiresTime) { expiresTime_ = expiresTime; }
    private:        
        bool selectDb();

    private:
        int type_;
        int encoding_;
        int db_num_;
        std::string key_;
        std::string value_;
        long long expiresTime_;
        //键空间中的实际对象
        std::map <std::pair<std::string, long long>, std::string> String_;
        std::map <std::pair<std::string, long long>, std::map<std::string, std::string>> Hash_;
        std::map <std::pair<std::string, long long>, std::list<std::string>> List_;
};