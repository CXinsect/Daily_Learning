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
    if(res.c_str() == NULL) {
        return Status::IOError("Empty Content").ToString();
    }
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
        return Status::IOError("Set error").ToString();
    } else {
        return Status().ToString();
    }
}
const std::string Server::bgsaveCommand(){
    std::shared_ptr <Persistence> p = persistence_.lock();
    if(p) {
        bool flags = p->CheckStorageConditions();
        if(flags)
            return Status::Ok().ToString();
    } else {
         std::cout << "This Line" << std::endl;
         std::shared_ptr<DataBase> tmp (&database_[db_index_],endDataBase);
         p.reset(new Persistence(tmp,db_len_));
         bool flags = p->CheckStorageConditions();
      if(flags)
            return Status::Ok().ToString();
        else
        return Status::IOError("Persistence Error").ToString();
    }
    return Status::IOError("Persistence Error").ToString();
}
const std::string Server::delCommand(const std::string &key) {
    bool flags = database_[db_index_].delKeySpace(DataStructure::ObjString,key);
    if(flags) return Status::Ok().ToString();
    else return Status::IOError("Delete Error").ToString();
}
const std::string Server::selectCommand(const std::string &index_) {
    int index = atoi(index_.c_str());
    DataBase database;
    database_.push_back(database);
    db_len_ = database_.size();
    db_index_ = index - 1;
    std::shared_ptr<DataBase> tmp(&database_[db_index_],endDataBase);
    std::weak_ptr<Persistence> pbase(std::shared_ptr<Persistence>(new Persistence (tmp,db_len_)));
    persistence_ = pbase;
    return Status::Ok().ToString();
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
    if(flags) return Status::Ok().ToString();
    else return Status::IOError("Expire Error").ToString();
}
const std::string Server::hsetCommand(const std::string &key, const std::string &value,const std::string &value1) {
    assert(db_index_ >= 0);
    std::cout << "ddddd" << key << value << value1 << std::endl;
    bool flags = database_[db_index_].addKeySpace(DataStructure::ObjHash,
                                                       DataStructure::EncodingRaw,
                                                        key,value,value1,
                                                        DefaultTime);
    if(flags) return Status::Ok().ToString();
    else return Status::IOError("Expire Error").ToString();

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
    std::string transition;
    while(it != database_[db_index_].getKeySpaceHashObject().end()) {
        tmp = database_[db_index_].getKeySpace(DataStructure::ObjHash,it->first.first);
        transition = it->first.first + ": " + tmp + " ";
        res += transition;
        it++;
    }
    std::cout << "Server[hgetall]: " << res << std::endl;
    if(res.size() == 0) res = Status::NotFound("Not Found hgetall").ToString();
    return res;
}
const std::string Server::rpushCommand (const std::string &key,const std::string &value) {
    assert(db_index_ >= 0);
    std::string res = std::string();
    std::istringstream str(value);
    std::string tmp;
    bool flags;
    while(str >> tmp) {
        std::cout << "test rpush " << tmp << std::endl;
        flags = database_[db_index_].addKeySpace(DataStructure::ObjList,
                                                       DataStructure::EncodingRaw,
                                                        key,tmp,DataStructure::SpareTire,
                                                        DefaultTime);
    }
    if(flags) res = Status::Ok().ToString();
    else res = Status::IOError("Rpush Error").ToString();
    return res;
}
const std::string Server::rpopCommand(const std::string &key) {
    assert(db_index_ >= 0);
    std::string res = database_[db_index_].delListObject(key);
    if(res.size() >= 0) return res;
    else return Status::IOError("Rpop Error").ToString();
}
std::string Server::commandRequest(Buffer *buf) {
    std::string res = std::string();
    std::string org = buf->retrieveAllAsString();
    std::cout << "Request: " << org << std::endl;
//    int pos = org.find('!',0);
//    int ret = org.find('#',pos);
//    int len = atoi(org.substr(pos+1,ret).c_str());
    string dst = "\r\n";
    int ret = org.find(dst);
    cmdLen_ = atoi(org.substr(1,ret).c_str());
    int pos = org.find('$',0);
    ret = org.find(dst.c_str(),pos,2);
    keylen_ = atoi(org.substr(pos+1,ret).c_str());
    cmd_ = org.substr(ret+dst.size(),keylen_);
    if (cmd_ == "get") {
        std::vector<cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end()) {
            if(it->name == cmd_) break; 
            else it++;
        }
        if(it == cmdtable_.end())
            res = Status::NotFound("Not Found get").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            // std::string t = key_;
            res = it->callback(key_,"","","","","");
        }

    } else if (cmd_ == "set") {
        std::vector <cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end())
            res = Status::NotFound("Not Found set").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            valuelen_ = atoi(org.substr(pos+1,ret).c_str());
            value_ = org.substr(ret+dst.size(),valuelen_);
            res = it->callback(key_,value_,"","","","");
        }

    } else if (cmd_ == "bgsave") {
        std::vector <cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) 
            res = Status::NotFound("Not Found This Command bgsave").ToString();
        else 
            res = it->callback("","","","","","");

    } else if (cmd_ == "del") {
        std::vector <cmdTable>:: iterator it = cmdtable_.begin();
        while (it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command del").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            res = it->callback(key_,"","","","","");
        }
    } else if (cmd_ == "select") {
        std::vector<cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command select").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            res = it->callback(key_,"","","","","");
        }
    } else if (cmd_ == "expire") {
        std::vector<cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command expire").ToString();
        else {
           pos = org.find('$',ret);
           ret = org.find(dst.c_str(),pos,2);
           keylen_ = atoi(org.substr(pos+1,ret).c_str());
           key_ = org.substr(ret+dst.size(),keylen_);
           pos = org.find('$',ret);
           ret = org.find(dst.c_str(),pos,2);
           valuelen_ = atoi(org.substr(pos+1,ret).c_str());
           value_ = org.substr(ret+dst.size(),valuelen_);
           res = it->callback(key_,value_,"","","","");
        }
    } else if (cmd_ == "hset") {
        std::vector<cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command hset").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            skeylen_ = atoi(org.substr(pos+1,ret).c_str());
            skey_ = org.substr(ret+dst.size(),skeylen_);
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            valuelen_ = atoi(org.substr(pos+1,ret).c_str());
            value_ = org.substr(ret+dst.size(),valuelen_);
            std::cout << "value: " << value_ << "size: " << value_.size() << std::endl;
            res = it->callback(key_,skey_,value_,"","","");
        }
    } else if(cmd_ == "hget") {
        std::vector <cmdTable>::iterator it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command hget").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            res = it->callback(key_,"","","","","");
        }
    } else if (cmd_ == "rpop") {
        auto it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command rpop").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            res = it->callback(key_,"","","","","");
        }
    } else if (cmd_ == "rpush") {
        auto it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command rpush").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            valuelen_ = atoi(org.substr(pos+1,ret).c_str());
            value_ = org.substr(ret+dst.size(),valuelen_);
            res = it->callback(key_,value_,"","","","");
        }
    } else if (cmd_ == "hgetall") {
        auto it = cmdtable_.begin();
        while(it != cmdtable_.end() && it->name != cmd_) it++;
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command hgetall").ToString();
        else 
            res = it->callback("","","","","","");

    } else {
        std::cout << "[Server::commandRequest:] No Command " << std::endl;
        res = Status::InvalidArgument("No Command").ToString();
    }
    return res;
}