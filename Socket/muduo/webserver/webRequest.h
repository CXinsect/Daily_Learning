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
#include "Buffer.h"
#include "disCription.h"
class webRequest :public disCription {
    public:

        webRequest(): method_(GET),url_(NULL),version_(NULL),
                      checkIndex_(0),startLine_(0),checkstate_(CheckStateRequestLine),
                      host_(NULL),messageLength_(0),link_(false) {}
                      
        LineStatus parseLine();
        HttpCode parseRequestLine (std::string& text);
        HttpCode parseHeader(std::string& text);
        HttpCode parseContext(std::string& text);
        HttpCode eventProcess();
    private:
        Method method_;
        std::string url_;
        std::string version_;
        int checkIndex_;
        int startLine_;
        CheckState checkstate_;
        // std::string fileName_;
        std::string host_;
        int messageLength_;
        int contentLength_;
        bool link_;
        struct stat filestat_;
        std::string requeseBuffer_;
        int conn_;
        Buffer buffer_;
};
#endif