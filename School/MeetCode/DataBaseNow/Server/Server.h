#ifndef _SERVER_H_
#define _SERVER_H_

#include "Accept.h"
#include "EventLoop.h"
// #include "dataBase.cc"
#include "redisPersistence.h"

#include "./util/status.h"
#include "dataBase.h"
#include <unordered_map>

using namespace _Redis;

class Server {
    public:
        Server(EventLoop *loop,const Address &peeraddr) : 
                    loop_(loop),
                    accept_(std::shared_ptr<Accept>(new Accept(loop,peeraddr))),
                    persistence_(std::shared_ptr<Persistence>(new Persistence))
        { 
            accept_->setConnectionCallBack(std::bind(&Server::onConnection,this,_1));
            accept_->setMessageCallBack(std::bind(&Server::onMessage,this,_1,_2,_3));
            accept_->listen();
            Init();
        }
    private:
        const std::string getCommand(const vector<string>&);
        const std::string setCommand(const vector<string>&);
        const std::string bgsaveCommand(const vector<string>&);
        const std::string delCommand(const vector<string>&);
        const std::string selectCommand(const vector<string>&);
        const std::string expireTimeCommand(const vector<string>&);
        const std::string rpushCommand(const vector<string>&);
        const std::string rpopCommand(const vector<string>&);
        const std::string hsetCommand(const vector<string>&);
        const std::string hgetCommand(const vector<string>&);
        const std::string hgetallCommand(const vector<string>&);
        static void endDataBase(DataBase *) { ; }
    public:
        void Init() {
            DataBase database;
            //Load local file
            database.rdbLoad();
            std::cout << "size: " << database.getKeySpaceStringSize() << std::endl;
            database_.push_back(database);
            //update database index
            db_len_ = database_.size();
            db_index_ = database_.size()-1;
            std::shared_ptr <DataBase> tmp(&database,endDataBase);
            std::weak_ptr<Persistence> pbase(std::shared_ptr<Persistence>(new Persistence (tmp,db_len_)));
            persistence_ = pbase;
            //init command table
            cmdtable_.insert(make_pair("get",std::bind(&Server::getCommand,this,_1)));
            cmdtable_.insert(make_pair("set",std::bind(&Server::setCommand,this,_1)));
            cmdtable_.insert(make_pair("bgsave",std::bind(&Server::bgsaveCommand,this,_1)));
            cmdtable_.insert(make_pair("del",std::bind(&Server::delCommand,this,_1)));
            cmdtable_.insert(make_pair("select",std::bind(&Server::selectCommand,this,_1)));
            cmdtable_.insert(make_pair("expire",std::bind(&Server::expireTimeCommand,this,_1)));
            cmdtable_.insert(make_pair("rpush",std::bind(&Server::rpushCommand,this,_1)));
            cmdtable_.insert(make_pair("rpop",std::bind(&Server::rpopCommand,this,_1)));
            cmdtable_.insert(make_pair("hset",std::bind(&Server::hsetCommand,this,_1)));
            cmdtable_.insert(make_pair("hget",std::bind(&Server::hgetCommand,this,_1)));
            cmdtable_.insert(make_pair("hgetall",std::bind(&Server::hgetallCommand,this,_1)));
        }
        void onConnection(const AcceptorPtr& conn);
        void onMessage(const AcceptorPtr& conn,Buffer *buf,ssize_t n);
        std::string commandRequest(Buffer *buf);
        void commandReply(Buffer *buf);


    private:
        EventLoop *loop_;
        std::shared_ptr<Accept> accept_;
        unordered_map <string,function<string(const vector<string>&)>> cmdtable_;
        
        int db_index_  = -1;
        int db_len_;
        int max_index_ = 16;
        std::vector <DataBase> database_;
        std::weak_ptr<Persistence> persistence_;
        //temporary structure
        int keylen_;
        int skeylen_;
        int valuelen_;
        std::string cmd_;
        std::string key_;
        std::string skey_;
        std::string value_;
public:
        int cmdLen_;
private:
    Status status_;
};


#endif