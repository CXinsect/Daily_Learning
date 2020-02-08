#include "Server.h"
// #include "dataBase.h"

void Server::onConnection(const AcceptorPtr &conn) {
    std::cout << "New Connection" << std::endl;
}
void Server::onMessage(const AcceptorPtr &conn,Buffer *buf,ssize_t len) {
    std::string res = commandRequest(buf);
    std::cout << "Response has been sent: " << res << std::endl;
    conn->send(res);
}
const std::string  Server::getCommand (const vector<string>& argv) {

    std::cout << "getCommand: " << argv[1] <<  std::endl;
    if(argv.size() != 2) return Status::IOError("Wrong parameter").ToString();
    std::string res = database_[db_index_].getKeySpace(DataStructure::ObjString,argv[1]);
    if(res.c_str() == NULL) {
        return Status::IOError("Empty Content").ToString();
    }
    else
        return res;
}
const std::string Server::setCommand (const vector<string>& argv) {
    if(argv.size() != 3) return Status::IOError("Wrong parameter").ToString();
    bool flags = database_[db_index_].addKeySpace(DataStructure::ObjString,
                                                       DataStructure::EncodingRaw,
                                                        argv[1],argv[2],
                                                        DataStructure::SpareTire,
                                                        DefaultTime);
    std::cout << "size: " << database_[db_index_].getKeySpaceStringSize() << std::endl;
    if(!flags) {
        return Status::IOError("Set error").ToString();
    } else {
        return Status().ToString();
    }
}
const std::string Server::bgsaveCommand(const vector<string>& argv){
    if(argv.size() != 0) return Status::IOError("Wrong parameter").ToString();
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
const std::string Server::delCommand(const vector<string>& argv) {
    if(argv.size() != 1) return Status::IOError("Wrong parameter").ToString();
    bool flags = database_[db_index_].delKeySpace(DataStructure::ObjString,argv[0]);
    if(flags) return Status::Ok().ToString();
    else return Status::IOError("Delete Error").ToString();
}
const std::string Server::selectCommand(const vector<string>& argv) {
    if(argv.size() != 1) return Status::IOError("Wrong parameter").ToString();
    int index = atoi(argv[0].c_str());
    DataBase database;
    database_.push_back(database);
    db_len_ = database_.size();
    db_index_ = index - 1;
    std::shared_ptr<DataBase> tmp(&database_[db_index_],endDataBase);
    std::weak_ptr<Persistence> pbase(std::shared_ptr<Persistence>(new Persistence (tmp,db_len_)));
    persistence_ = pbase;
    return Status::Ok().ToString();
}
const std::string Server::expireTimeCommand(const vector<string>& argv) {
    assert(db_index_ >= 0);
    if(argv.size() != 2) return Status::IOError("Wrong parameter").ToString();
    vector<string> tmp;
    tmp.push_back(argv[1]);
    std::string value = getCommand(tmp);
    if(value == "Empty Reply") return value_;
    long long t = atoi(argv[1].c_str());
    bool flags = database_[db_index_].addKeySpace(DataStructure::ObjString,
                                                       DataStructure::EncodingRaw,
                                                        argv[1],value,
                                                        DataStructure::SpareTire,
                                                        t);
    if(flags) return Status::Ok().ToString();
    else return Status::IOError("Expire Error").ToString();
}
const std::string Server::hsetCommand(const vector<string>& argv) {
    assert(db_index_ >= 0);
    if(argv.size() != 4) return Status::IOError("Wrong parameter").ToString();  
    bool flags = database_[db_index_].addKeySpace(DataStructure::ObjHash,
                                                       DataStructure::EncodingRaw,
                                                        argv[1],argv[2],argv[3],
                                                        DefaultTime);
    if(flags) return Status::Ok().ToString();
    else return Status::IOError("Expire Error").ToString();

}
const std::string Server::hgetCommand (const vector<string>& argv) {
    assert(db_index_ >= 0);
    if(argv.size() != 2) return Status::IOError("Wrong parameter").ToString();
    std::string tmp = database_[db_index_].getKeySpace(DataStructure::ObjHash,argv[1]);
    return tmp;
}
const std::string Server::hgetallCommand (const vector<string>& argv) {
    assert(db_index_ >= 0);
    if(argv.size() != 0) return Status::IOError("Wrong parameter").ToString();  
    auto it = database_[db_index_].getKeySpaceHashObject().begin();
    std::string tmp,res;
    std::string transition;
    while(it != database_[db_index_].getKeySpaceHashObject().end()) {
        tmp = database_[db_index_].getKeySpace(DataStructure::ObjHash,it->first);
        transition = it->first + ": " + tmp + " ";
        res += transition;
        it++;
    }
    std::cout << "Server[hgetall]: " << res << std::endl;
    if(res.size() == 0) res = Status::NotFound("Not Found hgetall").ToString();
    return res;
}
const std::string Server::rpushCommand (const vector<string>& argv) {
    assert(db_index_ >= 0);
    if(argv.size() < 2) return Status::IOError("Wrong parameter").ToString();      
    std::string res = std::string();
    std::istringstream str(argv[2]);
    std::string tmp;
    bool flags;
    while(str >> tmp) {
        std::cout << "test rpush " << tmp << std::endl;
        flags = database_[db_index_].addKeySpace(DataStructure::ObjList,
                                                       DataStructure::EncodingRaw,
                                                        argv[1],tmp,DataStructure::SpareTire,
                                                        DefaultTime);
    }
    if(flags) res = Status::Ok().ToString();
    else res = Status::IOError("Rpush Error").ToString();
    return res;
}
const std::string Server::rpopCommand(const vector<string>& argv) {
    assert(db_index_ >= 0);
    if(argv.size() != 2) return Status::IOError("Wrong parameter").ToString();      
    std::string res = database_[db_index_].delListObject(argv[1]);
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
        vector<string> v;
        v.push_back(cmd_);
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end())
            res = Status::NotFound("Not Found get").ToString();
        else {
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            v.push_back(key_);
            res = it->second(v);
        }

    } else if (cmd_ == "set") {
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end())
            res = Status::NotFound("Not Found set").ToString();
        else {
            vector<string> v;
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            valuelen_ = atoi(org.substr(pos+1,ret).c_str());
            value_ = org.substr(ret+dst.size(),valuelen_);
            v.push_back(cmd_);
            v.push_back(key_);
            v.push_back(value_);
            res = it->second(v);
        }

    } else if (cmd_ == "bgsave") {
        auto it = cmdtable_.find(cmd_);
        vector <string> v;
        v.push_back(cmd_);
        if(it == cmdtable_.end()) 
            res = Status::NotFound("Not Found This Command bgsave").ToString();
        else 
            res = it->second(v);

    } else if (cmd_ == "del") {
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command del").ToString();
        else {
            vector<string> v;
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            v.push_back(cmd_);
            v.push_back(key_);
            res = it->second(v);
        }
    } else if (cmd_ == "select") {
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command select").ToString();
        else {
            vector<string> v;
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            v.push_back(cmd_);
            v.push_back(key_);
            res = it->second(v);
        }
    } else if (cmd_ == "expire") {
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command expire").ToString();
        else {
           vector<string> v;
           pos = org.find('$',ret);
           ret = org.find(dst.c_str(),pos,2);
           keylen_ = atoi(org.substr(pos+1,ret).c_str());
           key_ = org.substr(ret+dst.size(),keylen_);
           pos = org.find('$',ret);
           ret = org.find(dst.c_str(),pos,2);
           valuelen_ = atoi(org.substr(pos+1,ret).c_str());
           value_ = org.substr(ret+dst.size(),valuelen_);
           v.push_back(cmd_);
           v.push_back(key_);
           v.push_back(value_);
           res = it->second(v);
        }
    } else if (cmd_ == "hset") {
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command hset").ToString();
        else {
            vector<string> v;
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
            v.push_back(cmd_);
            v.push_back(key_);
            v.push_back(skey_);
            v.push_back(value_);
            std::cout << "value: " << value_ << "size: " << value_.size() << std::endl;
            res = it->second(v);
        }
    } else if(cmd_ == "hget") {
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command hget").ToString();
        else {
            vector<string> v;
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            v.push_back(cmd_);
            v.push_back(key_);
            res = it->second(v);
        }
    } else if (cmd_ == "rpop") {
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command rpop").ToString();
        else {
            vector<string> v;
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            v.push_back(cmd_);
            v.push_back(key_);
            res = it->second(v);
        }
    } else if (cmd_ == "rpush") {
        auto it = cmdtable_.find(cmd_);
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command rpush").ToString();
        else {
            vector<string> v;
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+dst.size(),keylen_);
            pos = org.find('$',ret);
            ret = org.find(dst.c_str(),pos,2);
            valuelen_ = atoi(org.substr(pos+1,ret).c_str());
            value_ = org.substr(ret+dst.size(),valuelen_);
            v.push_back(cmd_);
            v.push_back(key_);
            v.push_back(value_);
            res = it->second(v);
        }
    } else if (cmd_ == "hgetall") {
        auto it = cmdtable_.find(cmd_);
        vector<string> v;
        v.push_back(cmd_);
        if(it == cmdtable_.end()) res = Status::NotFound("Not Found This Command hgetall").ToString();
        else 
            res = it->second(v);

    }
     else {
        std::cout << "[Server::commandRequest:] No Command " << std::endl;
        res = Status::InvalidArgument("No Command").ToString();
    }
    return res;
}