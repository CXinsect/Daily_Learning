#include "Server.h"

void Server::onConnection(const AcceptorPtr &conn) {
    std::cout << "New Connection" << std::endl;
}
void Server::onMessage(const AcceptorPtr &conn,Buffer *buf,ssize_t len) {
    std::string res = commandRequest(buf);
    std::cout << "Response has been sent: " << res << std::endl;
    conn->send(res);
}
const std::string  Server::getCommand (const std::string &ptr) {

    std::cout << "getCommand: " << ptr <<  std::endl;
    std::string res = database_[db_index_].getKeySpace(DataStructure::ObjString,key_);
    if(res.c_str() == NULL)
        return "Empty Reply";
    else 
        return res;
}
const std::string Server::setCommand (const std::string &key,const std::string &value) {
    bool flags = database_[db_index_].addKeySpace(DataStructure::ObjString,
                                                       DataStructure::EncodingRaw,
                                                        key,value,
                                                        DataStructure::SpareTire,
                                                        DefaultTime);
    std::cout << "size: " << database_[db_index_].getKeySpaceStringSize() << std::endl;
    
    if(!flags) {
        return "set error";
    } else {
        return "ok";
    }
}
std::string Server::commandRequest(Buffer *buf) {
    std::string res = std::string();
    std::string org = buf->retrieveAllAsString();
    std::cout << "Request: " << org << std::endl;
    int pos = org.find('!',0);
    int ret = org.find('#',pos);
    int len = atoi(org.substr(pos+1,ret).c_str());
    cmd_ = org.substr(ret+1,len);
    if (cmd_ == "get") {
        std::vector<cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end()) {
            if(it->name == cmd_) break; 
            else it++;
        }
        if(it == cmdtable_.end())
            res = "Not Found This Command";
        else {
            pos = org.find('!',ret);
            ret = org.find('@',pos);
            keylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            key_ = org.substr(ret+1,keylen_);
            // std::string t = key_;
            res = it->callback(key_,"","","","","");
        }

    } else if (cmd_ == "set") {
        std::vector <cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end())
            res = "Not Found This Command";
        else {
            pos = org.find('!',ret);
            ret = org.find('@',pos);
            keylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            key_ = org.substr(ret+1,keylen_);
            pos = org.find('!',ret);
            ret = org.find('$',pos);
            valuelen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            value_ = org.substr(ret+1,valuelen_);
            res = it->callback(key_,value_,"","","","");
        }

    } else if (cmd_ == "bgsave") {

    } else if (cmd_ == "del") {

    } else if (cmd_ == "select") {

    } else if (cmd_ == "expire") {

    } else if (cmd_ == "rpush") {

    } else if (cmd_ == "rpop") {

    } else if (cmd_ == "hset") {

    } else if (cmd_ == "hget") {

    } else {
        std::cout << "[Server::commandRequest:] No Command " << std::endl;
        res = "No Command";
    }
    return res;
}