#ifndef _DISCRIPTION_H_
#define _DISCRIPTION_H_
#include <iostream>
#include <string>
class disCription {
    public:
        enum Method { GET = 0,POST,HEAD,PUT };
        enum CheckState { CheckStateRequestLine= 0,
                           CheckStateHeader,
                           CheckStateContent };
        enum HttpCode { NoRequest, GetRequest, BadRequest,
                         NoResource, ForbidenRequest, FileRequest,
                         InternalError,ClosedConnection };
        enum LineStatus { LineStatusOk,LineStatusBad,LineStatusOpen };

        const std::string defaultFilePath = "/home/insect";
        std::string filePath;

};
#endif