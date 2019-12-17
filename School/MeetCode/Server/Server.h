#ifndef _SERVER_H_
#define _SERVER_H_

#include "EventLoop.cc"
#include "Channel.cc"
#include "Accept.cc"
// #include "dataBase.cc"
#include "redisPersistence.cc"

#include "./util/status.h"

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
        const std::string getCommand(const std::string&);
        const std::string setCommand(const std::string&,const std::string&);
        const std::string bgsaveCommand();
        const std::string delCommand(const std::string&);
        const std::string selectCommand(const std::string&);
        const std::string expireTimeCommand(const std::string&,const std::string&);
        const std::string rpushCommand(const std::string&, const std::string&);
        const std::string rpopCommand(const std::string&);
        const std::string hsetCommand(const std::string&,const std::string&,const std::string&);
        const std::string hgetCommand(const std::string&);
        const std::string hgetallCommand();
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
            std::weak_ptr<Persistence> pbase(std::shared_ptr<Persistence>(new Persistence (tmp)));
            persistence_ = pbase;
            //init command table
            cmdtable_.push_back({"get",std::bind(&Server::getCommand,this,_1),2,"rF",0});
            cmdtable_.push_back({"set",std::bind(&Server::setCommand,this,_1,_2),4,"wm",0});
            cmdtable_.push_back({"bgsave",std::bind(&Server::bgsaveCommand,this),1,"as",0});
            cmdtable_.push_back({"del",std::bind(&Server::delCommand,this,_1),2,"w",0});
            cmdtable_.push_back({"select",std::bind(&Server::selectCommand,this,_1),3,"lF",0});
            cmdtable_.push_back({"expire",std::bind(&Server::expireTimeCommand,this,_1,_2),3,"wF",0});
            cmdtable_.push_back({"rpush",std::bind(&Server::rpushCommand,this,_1,_2),3,"wm",0});
            cmdtable_.push_back({"rpop",std::bind(&Server::rpopCommand,this,_1),2,"wm",0});
            cmdtable_.push_back({"hset",std::bind(&Server::hsetCommand,this,_1,_2,_3),4,"wm",0});
            cmdtable_.push_back({"hget",std::bind(&Server::hgetCommand,this,_1),2,"wm",0});
            cmdtable_.push_back({"hgetall",std::bind(&Server::hgetallCommand,this),2,"wm",0});
        }
        void onConnection(const AcceptorPtr& conn);
        void onMessage(const AcceptorPtr& conn,Buffer *buf,ssize_t n);
        std::string commandRequest(Buffer *buf);
        void commandReply(Buffer *buf);

    private:
        EventLoop *loop_;
        std::shared_ptr<Accept> accept_;
        std::vector <clientState> client_;
        std::vector<cmdTable> cmdtable_;
        int db_index_ = -1;
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