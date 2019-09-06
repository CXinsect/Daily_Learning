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
const std::string Server::bgsaveCommand(){
    std::shared_ptr <Persistence> p = persistence_.lock();
    if(p) {
        bool flags = p->CheckStorageConditions();
        if(flags)
            return "ok";
    } else {
         std::cout << "This Line" << std::endl;
         std::shared_ptr<DataBase> tmp (&database_[db_index_],endDataBase);
         p.reset(new Persistence(tmp));
         bool flags = p->CheckStorageConditions();
      if(flags)
            return "ok";
        else
        return "persistence error";
    }
    return "persistence error";
}
const std::string Server::delCommand(const std::string &key) {
    bool flags = database_[db_index_].delKeySpace(DataStructure::ObjString,key);
    if(flags) return "ok";
    else return "del error";
}
const std::string Server::selectCommand(const std::string &index_) {
    int index = atoi(index_.c_str());
    DataBase database;
    database_.push_back(database);
    db_len_ = database_.size();
    db_index_ = index - 1;
    std::shared_ptr<DataBase> tmp(&database_[db_index_],endDataBase);
    std::weak_ptr<Persistence> pbase(std::shared_ptr<Persistence>(new Persistence (tmp)));
    persistence_ = pbase;
    return "ok";
}
const std::string Server::expireTimeCommand(const std::string &key,const std::string &_time) {
    assert(db_index_ >= 0);
    std::string value = getCommand(key);
    if(value == "Empty Reply") return value_;
    long long t = atoi(_time.c_str());
    bool flags = database_[db_index_].addKeySpace(DataStructure::ObjString,
                                                       DataStructure::EncodingRaw,
                                                        key,value,
                                                        DataStructure::SpareTire,
                                                        t);
    if(flags) return "ok";
    else return "expire error";
}
const std::string Server::hsetCommand(const std::string &key, const std::string &value,const std::string &value1) {
    assert(db_index_ >= 0);
    std::cout << "ddddd" << key << value << value1 << std::endl;
    bool flags = database_[db_index_].addKeySpace(DataStructure::ObjHash,
                                                       DataStructure::EncodingRaw,
                                                        key,value,value1,
                                                        DefaultTime);
    if(flags) return "ok";
    else return "hset error";

}
const std::string Server::hgetCommand (const std::string &key) {
    assert(db_index_ >= 0);
    std::string tmp = database_[db_index_].getKeySpace(DataStructure::ObjHash,key);
    return tmp;
}
const std::string Server::hgetallCommand () {
    assert(db_index_ >= 0);
    auto it = database_[db_index_].getKeySpaceHashObject().begin();
    std::string tmp,res;
    while(it != database_[db_index_].getKeySpaceHashObject().end()) {
        tmp = database_[db_index_].getKeySpace(DataStructure::ObjHash,it->first.first);
        res += tmp;     
    }
    std::cout << "Server[hgetall]: " << res << std::endl;
    return res;
}
const std::string Server::rpushCommand (const std::string &key,const std::string &value) {
    assert(db_index_ >= 0);
    std::string res = std::string();
    std::istringstream str(value);
    std::string tmp;
    bool flags;
    while(str >> tmp) {
        std::cout << "test rpush " << std::endl;
        flags = database_[db_index_].addKeySpace(DataStructure::ObjList,
                                                       DataStructure::EncodingRaw,
                                                        key,tmp,DataStructure::SpareTire,
                                                        DefaultTime);
    }
    if(flags) res = "ok";
    else res = "rpush error";
    return res;
}
const std::string Server::rpopCommand(const std::string &key) {
    assert(db_index_ >= 0);
    std::string res = database_[db_index_].delListObject(key);
    if(res.size() >= 0) return res;
    else return "rpop error";
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
        std::vector <cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) 
            res = "Not Found This Command";
        else 
            res = it->callback("","","","","","");

    } else if (cmd_ == "del") {
        std::vector <cmdTable>:: iterator it = cmdtable_.begin();
        while (it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = "Not Found This Command";
        else {
            pos = org.find('!',ret);
            ret = org.find('@',pos);
            keylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            key_ = org.substr(ret+1,keylen_);
            res = it->callback(key_,"","","","","");
        }
    } else if (cmd_ == "select") {
        std::vector<cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = "Not Found This Command";
        else {
            pos = org.find('!',ret);
            ret = org.find('@',pos);
            keylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            key_ = org.substr(ret+1,keylen_);
            res = it->callback(key_,"","","","","");
        }
    } else if (cmd_ == "expire") {
        std::vector<cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = "Not Found This Command";
        else {
           pos = org.find('!',ret);
           ret = org.find('@',pos);
           keylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
           key_ = org.substr(ret+1,keylen_);
           pos = org.find('!',ret);
           ret = org.find('@',pos);
           valuelen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
           value_ = org.substr(ret+1,valuelen_);
           res = it->callback(key_,value_,"","","","");
        }
    } else if (cmd_ == "hset") {
        std::vector<cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = "Not Found This Command";
        else {
            pos = org.find('!',ret);
            ret = org.find('#',pos);
            keylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            key_ = org.substr(ret+1,keylen_);
            pos = org.find('!',ret);
            ret = org.find('@',pos);
            skeylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            skey_ = org.substr(ret+1,skeylen_);
            pos = org.find('!',ret);
            ret = org.find('$',pos);
            valuelen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            value_ = org.substr(ret+1,skeylen_);
            res = it->callback(key_,skey_,value_,"","","");
        }
    } else if(cmd_ == "hget") {
        std::vector <cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = "Not Found This Command";
        else {
            pos = org.find('!',ret);
            ret = org.find('@',pos);
            keylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            key_ = org.substr(ret+1,keylen_);
            res = it->callback(key_,"","","","","");
        }
    } else if (cmd_ == "rpop") {
        auto it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = "Not Found This Command";
        else {
            pos = org.find('!',ret);
            ret = org.find('@',pos);
            keylen_ = atoi(org.substr(pos+1,ret-pos-1).c_str());
            key_ = org.substr(ret+1,keylen_);
            res = it->callback(key_,"","","","","");
        }
    } else if (cmd_ == "rpush") {
        auto it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = "Not Found This Command";
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
    } else if (cmd_ == "hgetall") {
        auto it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = "Not Found This Command";
        else 
            res = it->callback("","","","","","");

    } else {
        std::cout << "[Server::commandRequest:] No Command " << std::endl;
        res = "No Command";
    }
    return res;
}