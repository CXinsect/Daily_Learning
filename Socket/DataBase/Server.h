#ifndef _SERVER_H_
#define _SERVER_H_

#include "EventLoop.cc"
#include "Channel.cc"
#include "Accept.cc"

class Server {
    public:
        Server(EventLoop *loop,const Address &peeraddr) : 
                    loop_(loop),
                    accept_(std::shared_ptr<Accept>(new Accept(loop,peeraddr)))
        { 
            accept_->setConnectionCallBack(std::bind(&Server::onConnection,this,_1));
            accept_->setMessageCallBack(std::bind(&Server::onMessage,this,_1,_2,_3));
        }
    private:
        void getCommand(const std::string&);
        void setCommand(const std::string&,const std::string&,const std::string&);
        void bgsaveCommand(const std::string&);
        void delCommand(const std::string&,const std::string&);
        void selectCommand(const std::string&,const std::string&);
        void expireTimeCommand(const std::string&,const std::string&,const std::string&);
        void rpushCommand(const std::string&,const std::string&,const std::string&);
        void rpopCommand(const std::string&,const std::string&);
        void hsetCommand(const std::string&,const std::string&,const std::string&,const std::string&);
        void hgetCommand(const std::string&,const std::string&);
    public:
        void Init() {
            cmdtable_.push_back({"get",std::bind(&Server::getCommand,this,_1),2,"rF",0});
            cmdtable_.push_back({"set",std::bind(&Server::setCommand,this,_1,_2,_3),4,"wm",0});
            cmdtable_.push_back({"bgsave",std::bind(&Server::bgsaveCommand,this,_1),2,"as",0});
            cmdtable_.push_back({"del",std::bind(&Server::delCommand,this,_1,_2),3,"w",0});
            cmdtable_.push_back({"select",std::bind(&Server::selectCommand,this,_1,_2),3,"lF",0});
            cmdtable_.push_back({"expire",std::bind(&Server::expireTimeCommand,this,_1,_2,_3),4,"wF",0});
            cmdtable_.push_back({"rpush",std::bind(&Server::rpushCommand,this,_1,_2,_3),4,"wm",0});
            cmdtable_.push_back({"rpop",std::bind(&Server::rpopCommand,this,_1,_2),3,"wm",0});
            cmdtable_.push_back({"hset",std::bind(&Server::hsetCommand,this,_1,_2,_3,_4),5,"wm",0});
            cmdtable_.push_back({"hget",std::bind(&Server::hgetCommand,this,_1,_2),3,"wm",0});
        }
        void onConnection(const AcceptorPtr& conn);
        void onMessage(const AcceptorPtr& conn,Buffer *buf,ssize_t n);
        void commandRrequest(Buffer *buf);
        void commandReply(Buffer *buf);

    private:
        EventLoop *loop_;
        std::shared_ptr<Accept> accept_;
        std::vector <clientState> client_;
        std::vector<cmdTable> cmdtable_;
};
#endif