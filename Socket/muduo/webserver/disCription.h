#ifndef _DISCRIPTION_H_
#define _DISCRIPTION_H_
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class disCription {
    public:
        enum Method { GET = 0,POST,HEAD,PUT };
        enum CheckState { CheckStateRequestLine= 0,
                           CheckStateHeader,
                           CheckStateContent };
        enum HttpCode { NoRequest, GetRequest, BadRequest,
                         NoResource, ForbidenRequest, FileRequest,
                         DirRequest,InternalError,ClosedConnection };
        enum LineStatus { LineStatusOk,LineStatusBad,LineStatusOpen };

        const std::string defaultFilePath = "/home/insect";
        std::string filePath;
        bool link_ = false;
        static struct stat st_;
};
struct stat disCription::st_;
#endif