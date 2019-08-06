#ifndef _RESPONSE_H_
#define _RESPONSE_H_
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
// #include "Buffer.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "disCription.h"
#include "webRequest.h"
const std::string Version = "HTTP/1.1";

class webResponse : public disCription {
 public:
  const std::string Ok = "ok";
  const std::string _400 = "Bad Requeest";
  const std::string _403 = "Forbidden";
  const std::string _404 = "Not Found";
  const std::string _500 = "Internal Error";
  HttpCode requestAction(void);
  // bool fileResponseWrite(const TcpConnectionPtr &conn_,Buffer*buffer_);
  void fileResponseAddHead(Buffer *buffer_,int length_);
  bool fileResponseAssembly(Buffer *buffer_);
  void setHttpCodeStatus(HttpCode status) { httpcodestatus_ = status; }
  std::string getFileType();
  ~webResponse() {
    while (count_) {
      munmap(fileAddr, BuffSize);
      count_--;
      flagsAddr -= BuffSize;
    }
    flagsAddr = NULL;
  }
  static char* fileAddr;
  static char *flagsAddr;
  static int count_;
 private:
  webRequest request_;
  // Buffer buffer_;
  // TcpConnection conn_;
  char buf_[64];
  HttpCode httpcodestatus_;
  std::string title_;
};
char *webResponse::fileAddr = NULL;
char *webResponse::flagsAddr = NULL;
int webResponse::count_ = 0;
#endif