#ifndef _RESPONSE_H_
#define _RESPONSE_H_
#include <iostream>
#include <string>
#include "webRequest.h"
#include "disCription.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "Buffer.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
class webResponse :public disCription{
    public:
        const std::string Ok = "ok";
        const std::string _400 = "Bad Requeest";
        const std::string _403 = "Forbidden";
        const std::string _404 = "Not Found";
        const std::string _500 = "Internal Error";
        HttpCode  requestAction (void);
        bool response();
        HttpCode fileRequest(void);
    private:
        int writeIndex_;
        std::string fileAddr;
        webRequest request_;
        Buffer buffer_;
        int conn_;
        std::string title_;
        int status_;
};
#endif