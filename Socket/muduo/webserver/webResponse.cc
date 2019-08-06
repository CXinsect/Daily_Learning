#include "webResponse.h"

void webResponse::fileResponseAddHead(Buffer *buffer_,int length_) {
   std::cout << "hello" << std::endl;
    memset(buf_,0,sizeof(buf_));
    snprintf(buf_,sizeof(buf_),"Content-Length: %d\r\n",length_);
    buffer_->Append(buf_,strlen(buf_));
    memset(buf_,0,sizeof(buf_));
    snprintf(buf_,sizeof(buf_),"Connection: %s\r\n",(link_ == true) ? "keep-alive" : "close");
    buffer_->Append(buf_,strlen(buf_));
    memset(buf_,0,sizeof(buf_));
    snprintf(buf_,sizeof(buf_),"\r\n");
    buffer_->Append(buf_,strlen(buf_));
    // buffer_.Append(fileAddr,strlen(fileAddr));
}
bool webResponse::fileResponseAssembly(Buffer *buffer_) {
    std::cout << "fileresponse " << std::endl;
    switch(httpcodestatus_) {
        case InternalError: {
            memset(buf_,0,sizeof(buf_));
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),500,_500.c_str());
            buffer_->Append(buf_,strlen(buf_));
            fileResponseAddHead(buffer_,_500.size());
             const std::string emptyFile = "<html><body></body></html>";
            buffer_->Append(emptyFile.c_str(),emptyFile.size());
            return true;
        }
        case BadRequest: {
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),400,_400.c_str());
            buffer_->Append(buf_,strlen(buf_));
            fileResponseAddHead(buffer_,_400.size());
            buffer_->Append(_400.c_str(),_400.size());
            return true;
        }
        case NoResource: {
            memset(buf_,0,sizeof(buf_));
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),404,_404.c_str());
            buffer_->Append(buf_,strlen(buf_));
            fileResponseAddHead(buffer_,_404.size());
            buffer_->Append(_404.c_str(),_404.size());
            return true;
        }
        case ForbidenRequest: {
            memset(buf_,0,sizeof(buf_));
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),403,_403.c_str());
            buffer_->Append(buf_,strlen(buf_));
            fileResponseAddHead(buffer_,_403.size());
            buffer_->Append(_403.c_str(),_403.size());
            return true;
        }
        case FileRequest: {
            memset(buf_,0,sizeof(buf_));
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),200,Ok.c_str());
            std::cout << strlen(buf_) << std::endl;
            std::string tmp = buf_;
            // std::cout << buffer_->retrieveAllAsString() << std::endl;
            buffer_->Append(tmp.c_str(),tmp.size());
            if(st_.st_size != 0) {
                fileResponseAddHead(buffer_,st_.st_size);
                std::cout << "filesize: " << strlen(fileAddr) << std::endl;
                buffer_->Append(fileAddr,strlen(fileAddr));
            }
            else {
                const std::string emptyFile = "<html><body></body></html>";
                buffer_->Append(emptyFile.c_str(),emptyFile.size());
                buffer_->Append(fileAddr,strlen(fileAddr));
            }
            return true;
        }
        default: {
            std::cout << "webResponse::fileresponse Unknown option" << std::endl;
            return false;
        }
    }
}
std::string webResponse::getFileType() {
    const char* dot;

    // 自右向左查找‘.’字符, 如不存在返回NULL
    dot = strrchr(filename.c_str(), '.');   
    if (dot == NULL)
        return "text/plain; charset=utf-8";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot,".png") == 0)
        return "image/png";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp( dot, ".wav" ) == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}
