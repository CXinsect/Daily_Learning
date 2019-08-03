#include "webRequest.h"


webRequest::LineStatus webRequest::parseLine() {
    char temp;
    for(;checkIndex_ < messageLength_;checkIndex_++) {
        temp = requeseBuffer_[checkIndex_];
        if(temp == '\r') {
            if(checkIndex_ + 1 == messageLength_) return LineStatusOpen;
            else if(requeseBuffer_[checkIndex_ + 1] == '\n') {
                requeseBuffer_[checkIndex_++] = '\0';
                requeseBuffer_[checkIndex_++] = '\0';
                return LineStatusOpen;
            }
            return LineStatusBad;
        }
        else if(temp == '\n') {
            if(checkIndex_ > 1 && requeseBuffer_[checkIndex_ - 1] == '\r') {
                requeseBuffer_[checkIndex_-1] = '\0';
                requeseBuffer_[checkIndex_++] = '\0';
                return LineStatusOk;
            }
            return LineStatusBad;
        }
    }
    return LineStatusOpen;
}
webRequest::HttpCode webRequest::parseRequestLine (std::string& text) {
    int pos = text.find_first_of(" \t");
    std::string method = text.substr(0,pos);
    if(method == "GET") {
        method_ = GET;
    } else return BadRequest;
    url_ = text.substr(pos+1,text.size());
    int pos2 = url_.find_first_of(" ");
    version_ = url_.substr(pos2+1,url_.size());
    url_ = url_.substr(0,pos2);
    if(url_.size() == 0) return BadRequest;
    int pos3 = version_.find_first_of(" ");
    version_ = version_.substr(0,pos3);
    if(version_.size() == 0) return BadRequest;
    if(version_ != "HTTP/1.1") return BadRequest;
    if(strncasecmp(url_.c_str(),"http://",7) == 0) {
        url_ = url_.substr(6,url_.size());
        std::cout << "url :" << url_[0]<< std::endl;
    } 
    if(url_.size() == 0 || url_[0] != '/') return BadRequest;
    checkstate_ = CheckStateHeader;
    return NoRequest; 
}
webRequest::HttpCode webRequest::parseHeader(std::string& text) {
    if(text[0] == '0') {
        if(messageLength_ != 0) {
            checkstate_ = CheckStateContent;
            return NoRequest;
        }
        return GetRequest;
    }
    else if(strncasecmp(text.c_str(),"Connection:",11) == 0) {
        text = text.substr(11,text.size());
        int pos = text.find_first_not_of(" ");
        text = text.substr(pos,text.size());
        std::cout << "Connection: " << text << std::endl;
        if(strcasecmp(text.c_str(),"keep-alive") == 0)
            link_ = true;
    }
    else if(strncasecmp(text.c_str(),"Content-Length:",15) == 0) {
        text = text.substr(15,text.size());
        int pos = text.find_first_of(" ");
        text = text.substr(pos,text.size());
        contentLength_ = atol(text.c_str());
        std::cout << "Content-Length: " << contentLength_ << std::endl;
    }
    else if(strncasecmp(text.c_str(),"Host",5) == 0) {
        text = text.substr(5,text.size());
        int pos = text.find_first_not_of(" ");
        text = text.substr(pos,text.size());
        host_ = text;
    }
    else {
        std::cout << "Temporarily unprocessed" << std::endl;
    }
    return NoRequest;
}
webRequest::HttpCode webRequest::parseContext(std::string& text) {
    if((contentLength_ + checkIndex_) > messageLength_) {
        return GetRequest;
    }
    return NoRequest;
}

webRequest::HttpCode webRequest::eventProcess() {
    LineStatus linestatus = LineStatusOk;
    HttpCode httpcode = NoRequest;
    std::string text;
    while(((checkstate_ == CheckStateContent) &&
            (linestatus == LineStatusOk)) ||
            ((linestatus = parseLine()) == LineStatusOk))
    {
        text = buffer_.retrieveAllAsString();
        startLine_ = checkIndex_;
        std::cout << "The date in Readbuffer: " << text << std::endl;
        switch(checkstate_) {
            case CheckStateRequestLine: {
                httpcode = parseRequestLine(text);
                if(httpcode == BadRequest) return BadRequest;
                break;
            }
            case CheckStateHeader: {
                httpcode = parseHeader(text);
                if(httpcode == BadRequest) return BadRequest;
                else if(httpcode == GetRequest) //处理请求
                break;
            }
            case CheckStateContent: {
                httpcode = parseContext(text);
                if(httpcode == GetRequest) //处理请求
                linestatus = LineStatusOpen;
                break;
            }
            default:
                return InternalError;
        }
    }
        return NoRequest;
}