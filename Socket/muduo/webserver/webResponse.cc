#include "webResponse.h"

void webResponse::fileResponseAddHead(Buffer *buffer_,int length_) {
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
void webResponse::fileResponseAssembly(Buffer *buffer_) {
    switch(httpcodestatus_) {
        case InternalError: {
            memset(buf_,0,sizeof(buf_));
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),500,_500.c_str());
            buffer_->Append(buf_,strlen(buf_));
            fileResponseAddHead(buffer_,_500.size());
             const std::string emptyFile = "<html><body></body></html>";
            buffer_->Append(emptyFile.c_str(),emptyFile.size());
            break;
        }
        case BadRequest: {
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),400,_400.c_str());
            buffer_->Append(buf_,strlen(buf_));
            fileResponseAddHead(buffer_,_400.size());
            break;
        }
        case NoRequest: {
            memset(buf_,0,sizeof(buf_));
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),404,_404.c_str());
            buffer_->Append(buf_,strlen(buf_));
            fileResponseAddHead(buffer_,_404.size());
            break;
        }
        case ForbidenRequest: {
            memset(buf_,0,sizeof(buf_));
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),403,_403.c_str());
            buffer_->Append(buf_,strlen(buf_));
            fileResponseAddHead(buffer_,_403.size());
            break;
        }
        case FileRequest: {
            memset(buf_,0,sizeof(buf_));
            snprintf(buf_,sizeof(buf_),"%s %d %s\r\n",Version.c_str(),200,Ok.c_str());
            buffer_->Append(buf_,strlen(buf_));
            if(st_.st_size != 0) {
                fileResponseAddHead(buffer_,Ok.size());
                buffer_->Append(fileAddr,strlen(fileAddr));
            }
            else {
                const std::string emptyFile = "<html><body></body></html>";
                buffer_->Append(emptyFile.c_str(),emptyFile.size());
                buffer_->Append(fileAddr,strlen(fileAddr));
            }
            break;
        }
        default: {
            std::cout << "webResponse::fileresponse Unknown option" << std::endl;
            break;
        }
    }
}
