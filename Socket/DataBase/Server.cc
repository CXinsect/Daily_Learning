#include "Server.h"

void Server::onConnection(const AcceptorPtr &conn) {
    std::cout << "New Connection" << std::endl;
}
void Server::onMessage(const AcceptorPtr &conn,Buffer *buf,ssize_t len) {
    std::string test = commandRequest(buf);
    std::cout << "test: " << test << std::endl;
    conn->send(test);
}
void Server::getCommand (const std::string &ptr) {

    std::cout << "getCommand: " << ptr <<  std::endl;
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
            keylen_ = atoi(org.substr(pos+1,ret).c_str());
            key_ = org.substr(ret+1,keylen_);
            it->callback(key_,"","","","","");
            return "ok";
        }

    } else if (cmd_ == "set") {

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