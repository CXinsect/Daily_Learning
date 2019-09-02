#ifndef _SERVER_H_
#define _SERVER_H_

#include "EventLoop.cc"
#include "Channel.cc"
#include "Accept.cc"
// #include "dataBase.cc"
#include "redisPersistence.cc"
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
        void bgsaveCommand(const std::string&);
        void delCommand(const std::string&,const std::string&);
        void selectCommand(const std::string&,const std::string&);
        void expireTimeCommand(const std::string&,const std::string&,const std::string&);
        void rpushCommand(const std::string&,const std::string&,const std::string&);
        void rpopCommand(const std::string&,const std::string&);
        void hsetCommand(const std::string&,const std::string&,const std::string&,const std::string&);
        void hgetCommand(const std::string&,const std::string&);
        static void endDataBase(DataBase *) { ; }
    public:
        void Init() {
            DataBase database;
            //Load local file
            database.rdbLoad();
            std::cout << "size: " << database.getKeySpaceStringSize() << std::endl;
            database_.push_back(database);
            //update database index
            db_index_ = database_.size()-1;
            std::shared_ptr <DataBase> tmp(&database,endDataBase);
            std::weak_ptr<Persistence> pbase(std::shared_ptr<Persistence>(new Persistence (tmp)));
            persistence_ = pbase;
            //init command table
            cmdtable_.push_back({"get",std::bind(&Server::getCommand,this,_1),2,"rF",0});
            cmdtable_.push_back({"set",std::bind(&Server::setCommand,this,_1,_2),4,"wm",0});
            // cmdtable_.push_back({"bgsave",std::bind(&Server::bgsaveCommand,this,_1),2,"as",0});
            // cmdtable_.push_back({"del",std::bind(&Server::delCommand,this,_1,_2),3,"w",0});
            // cmdtable_.push_back({"select",std::bind(&Server::selectCommand,this,_1,_2),3,"lF",0});
            // cmdtable_.push_back({"expire",std::bind(&Server::expireTimeCommand,this,_1,_2,_3),4,"wF",0});
            // cmdtable_.push_back({"rpush",std::bind(&Server::rpushCommand,this,_1,_2,_3),4,"wm",0});
            // cmdtable_.push_back({"rpop",std::bind(&Server::rpopCommand,this,_1,_2),3,"wm",0});
            // cmdtable_.push_back({"hset",std::bind(&Server::hsetCommand,this,_1,_2,_3,_4),5,"wm",0});
            // cmdtable_.push_back({"hget",std::bind(&Server::hgetCommand,this,_1,_2),3,"wm",0});
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

};
#endif