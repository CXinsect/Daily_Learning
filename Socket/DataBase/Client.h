#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "model.h"

const int TimeValue = 60;
class Client {
    public:
        Client(int port,const std::string &ip,int _time):
                                port_(port),
                                Ip_(ip),
                                time_(_time)
                                {}
        int Connect();
        void sendRequest(const std::string &buf);
        void receiveReply(const std::string &buf);
    private:
        int setNoOrBlocking(int fd);
    private:
        int port_;
        const std::string Ip_;
        int confd_;
        int time_ = TimeValue;
};
#endif