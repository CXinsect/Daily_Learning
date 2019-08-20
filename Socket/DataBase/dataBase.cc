#include "dataBase.h"

void DataBase::addKeySpace (int type,int encoding, const std::string &key,const std::string &value,long long expiresTime) {
    type_ = type;
    encoding_ = encoding;
    key_ = key;
    value_ = value;
    expiresTime_ = expiresTime;
    if(type_ == DataStructure::ObjString) {
        String_.insert(make_pair(make_pair(key,expiresTime_),value_));
    }
    else if(type == DataStructure::ObjHash) {
        std::map <std::string, std::string> tmp;
        tmp.insert(make_pair(key_,value_));
        Hash_.insert(make_pair(make_pair(key_,expiresTime_),tmp));
    }
    else if(type == DataStructure::ObjList) {
        std::list<std::string> tmp;
        tmp.push_back(value_);
        List_.insert(make_pair(make_pair(key_,expiresTime_),tmp));
    }
    else {
        std::cout << "Unknown type" << std::endl;
        abort();
    }
    std::cout << "addKeySpace has been Executed" << std::endl;
}
void DataBase::delKeySpace(int type, const std::string &key) {
    if(type == DataStructure::ObjString) {
        std::map<std::pair<std::string,long long>,std::string >::iterator it = String_.begin();
        while(it != String_.end()) {
            if(it->first.first == key) {
                break;
            }
            else
                it++;
        }
        if(it == String_.end())
            std::cout << "Not Found" << std::endl;
        else {
            String_.erase(it);
        }
    }
    else if(type == DataStructure::ObjHash) {
        std::map<std::pair<std::string, long long>,std::map<std::string, std::string>>::iterator it = Hash_.begin();
        while(it != Hash_.end()) {
            if(it->first.first == key)
                break; else {
                    it++;
                }
        }
        if(it == Hash_.end()) {
            std::cout << "Not Found" << std::endl;
            abort();
        }
        else {
            Hash_.erase(it);
        }
    }
    else if(type == DataStructure::ObjList) {
        std::map<std::pair<std::string, long long>,std::list<std::string>> ::iterator it = List_.begin();
        while(it != List_.end()) {
            if(it->first.first == key)
                break; else {
                    it++;
                }
        }
        if(it == List_.end()) {
            std::cout << "Not Found" << std::endl;
            abort();
        }
        else List_.erase(it);
    }
    else {
        std::cout << "Unknown type" << std::endl;
        abort();
    }
}
std::string DataBase::getKeySpace(int type,const std::string &key) {
    std::string ret = std::string();
    if(type == DataStructure::ObjString) {
        std::map<std::pair<std::string,long long>,std::string >::iterator it = String_.begin();
        while(it != String_.end()) {
            if(it->first.first == key) {
                break;
            }
            else
                it++;
        }
        if(it == String_.end())
            ret = "Not Found";
            // std::cout << "Not Found" << std::endl;
        else ret = it->second;
    }
    else if(type == DataStructure::ObjHash) {
        std::map<std::pair<std::string, long long>,std::map<std::string, std::string>>::iterator it = Hash_.begin();
        while(it != Hash_.end()) {
            if(it->first.first == key)
                break; else {
                    it++;
                }
        }
        if(it == Hash_.end()) {
            std::cout << "Not Found" << std::endl;
            ret = "Not Found" ;
        }
        else {
            std::map<std::string, std::string>::iterator iter = it->second.find(key);
            ret = iter->second;
        }
    }
    else if(type == DataStructure::ObjList) {
        std::map<std::pair<std::string, long long>,std::list<std::string>> ::iterator it = List_.begin();
        while(it != List_.end()) {
            if(it->first.first == key)
                break; else {
                    it++;
                }
        }
        if(it == List_.end()) {
            std::cout << "Not Found" << std::endl;
            ret = "Not Found";
        }
        else {
            std::list<std::string> ::iterator iter = it->second.begin();
            ret = *iter;
        }
    }
    else {
        std::cout << "Unknown Type" << std::endl;
        ret = "Not Found";
    }
    std::cout << "getKeySpace: " << ret << std::endl;
    return ret;
}