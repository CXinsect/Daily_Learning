#ifndef _REQUEST_H_
#define _REQUEST_H_
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "TcpConnection.h"
#include "TcpServer.h"
#include "disCription.h"
#include "webResponse.h"
#include "FastCGI/FastCGI.cc"
class webRequest :public disCription {
    public:
        webRequest(): method_(GET),url_(std::string()),version_(std::string()),
                      checkIndex_(0),startLine_(0),checkstate_(CheckStateRequestLine),
                      host_(std::string()),messageLength_(0) {}
                  
        void setBuffer (Buffer &buffer) {
            buffer_ = buffer;
            requeseBuffer_ = buffer_.retrieveAllAsString();
        }
        LineStatus parseLine();
        HttpCode parseRequestLine (std::string& text);
        HttpCode parseHeader(std::string& text);
        HttpCode parseContext(std::string& text);
        HttpCode eventProcess(FastCGI &fastcgi);
        HttpCode requestAction();
        void fileRequestRead();
    private:
        Method method_;
        std::string methodcgi;
        std::string url_;
        std::string version_;
        int checkIndex_;
        int startLine_;
        CheckState checkstate_;
        // std::string fileName_;
        std::string host_;
        int messageLength_;
        int contentLength_;
        std::string contentlen_;
        std::string postContent_;
        std::string requeseBuffer_;
        TcpConnectionPtr conn_;
        Buffer buffer_;
        FastCGI fastcgi_;
};
class webResponse;
#endif