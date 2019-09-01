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
        
        void getInput() {
            char buf[1024] = {0};
            std::cout << "Input quit to Stop" << std::endl;
            while(fgets(buf,sizeof(buf),stdin) != NULL) {
                if(strcmp(buf,"quit") == 0)
                    break;
                std::cout << "test" << std::endl;
                const std::string tmp = buf;
                sendRequest(tmp);
                std::cout << "Input quit to Stop" << std::endl;
            }
        }
        void sendRequest(const std::string &buf);
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
            std::cout << "Response: " << buffer << ret << std::endl;
            memset(buffer,0,1024);
        }
    private:
        int port_;
        const std::string Ip_;
        int confd_;
        int time_ = TimeValue;
};
size_t Io::writen(int sockfd,char*buf,ssize_t count) {
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
size_t Io::readn(int sockfd,char*buf,ssize_t size) {
  assert(sockfd != -1);
  assert(buf != NULL);
  assert(count != 0);
  char *bufp = buf;
  int count = 0;
  while(1) {
    nread = read(sockfd,bufp,size);
    if(nread < 0) {
      if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
        break;
      }
      return -1;
    }
    else if(read == 0) {
      std::cout << "Server is Over" << std::endl;
      return count;
    }
    count += nread;
  }
  return count;
}
#endif