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
        ~Client() { close(confd_); }
        int Connect();
        void sendRequest(const std::string &buf);
        void getInput() {
            char buf[1024] = {0};
            std::cout << "Input quit to Stop" << std::endl;
            while(fgets(buf,sizeof(buf),stdin) != NULL) {
                if(strcmp(buf,"quit") == 0)
                    break;
                std::cout << "test" << std::endl;
                sendRequest(buf);
                std::cout << "Input quit to Stop" << std::endl;
            }
        }
        // void receiveReply(const std::string &buf);
    private:
        int setNoOrBlocking(int fd);
        void AuxiliaryFun(char *buffer) {
            int ret = Io::writen(confd_,buffer,strlen(buffer));
            assert(ret == strlen(buffer));
            memset(buffer,0,1024);
            setNoOrBlocking(confd_);
            ret = Io::readn(confd_,buffer,1024);
            assert(ret != -1);
            std::cout << "Response: " << buffer << std::endl;
            memset(buffer,0,1024);
        }
    private:
        int port_;
        const std::string Ip_;
        int confd_;
        int time_ = TimeValue;
};
size_t Io::writen(int sockfd,void*buf,ssize_t count) {
  assert(sockfd != -1);
  assert(buf != NULL);
  assert(count != 0);
  char *bufp = (char*)buf;
  size_t nleft = count;
  int nwrite = 0;
  while(nleft > 0) {
    nwrite = write(sockfd,bufp,nleft);
    if(nwrite < 0) {
      if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        continue;
        return -1;
    }
    if(nwrite == 0) continue;
    nleft -= nwrite;
    bufp += nwrite;
  }
  return count - nleft;
}
size_t Io::readn(int sockfd,void*buf,ssize_t count) {
  assert(sockfd != -1);
  assert(buf != NULL);
  assert(count != 0);
  char *bufp = (char*)buf;
  int nread = 0;
  int nleft = count;
  int old = 0;
  while(nleft > 0) {
    nread = read(sockfd,bufp,nleft);
    if(nread < 0) {
      if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) 
        return old;
      return -1;
    }
    if(nread == 0) {
      std::cout << "Server is Over" << std::endl;
      return nleft;
    }
    bufp += nread;
    nleft -= nread;
    old += nread;
  }
  return count - nleft;
}
#endif