#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <boost/implicit_cast.hpp>
#include <iostream>
#include <memory>
#include <thread>

class portScan {
 public:
  portScan() {}
  portScan(unsigned short s_port_, unsigned short d_port_,
           const struct in_addr ip_)
      : s_port(s_port_), d_port(d_port_), ip(ip_) {}
  bool doScan(struct sockaddr_in *addr);
  void doAction(portScan &p);
  static void Action(portScan &p) { p.doAction(p); }

 private:
  unsigned short s_port;
  unsigned short d_port;
  struct in_addr ip;
  struct sockaddr_in addr4;
};

bool portScan::doScan(struct sockaddr_in *addr) {
  int confd;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cout << "socket err" << std::endl;
    exit(0);
  }
  struct sockaddr *addr4 =
      static_cast<struct sockaddr *>(boost::implicit_cast<void *>(addr));
  int ret = connect(sockfd, addr4, sizeof(*addr));
  if (ret == 0) {
    std::cout << ntohs(addr->sin_port) << " has started "
              << "in ip " << inet_ntoa(addr->sin_addr) << std::endl;
    close(confd);
    return true;
  } else if (ret < 0 && errno == ECONNREFUSED) {
    std::cout << "port has stoped" << std::endl;
    close(confd);
    return false;
  } else {
    std::cout << "Other Err" << std::endl;
    return false;
  }
  return false;
}

void portScan::doAction(portScan & p) {
  struct sockaddr_in serv;
  memset(&serv, 0, sizeof(serv));
  serv.sin_addr.s_addr = p.ip.s_addr;
  serv.sin_family = AF_INET;
  for (int i = s_port; i <= d_port; i++) {
    serv.sin_port = htons(i);
    if (!doScan(&serv)) continue;
  }
  return;
}


int main(int argc, char *argv[]) {
  unsigned short maxport;
  unsigned short souport;
  int threadnum;
  struct in_addr ip;
  int seg_len;
  if (argc < 7) {
    std::cout << "[-m][maxport][-a][address][-n][thread]" << std::endl;
    exit(1);
  }
  // portScan p;
  for (int i = 1; i < 7; i++) {
    if (strcmp(argv[i], "-m") == 0) {
      maxport = atoi(argv[i + 1]);
      assert(maxport > 0 && maxport < 65535);
      continue;
    }
    if (strcmp(argv[i], "-a") == 0) {
      int ret = inet_aton(argv[i + 1], &ip);
      assert(ret != 0);
      continue;
    }
    if (strcmp(argv[i], "-n") == 0) {
      threadnum = atoi(argv[i + 1]);
      assert(threadnum > 0);
      continue;
    }
  }
  if (maxport < threadnum) threadnum = maxport;

  seg_len = maxport / threadnum;

  if (maxport % threadnum != 0) threadnum += 1;
  // pthread_t * ptr = new pthread_t [threadnum];
  // std::shared_ptr <pthread_t> threadaddr (ptr);
  portScan p_(souport, maxport, ip);
  for (int i = 0; i < threadnum; i++) {
    souport = i * seg_len + 1;
    if (i == threadnum - 1)
      maxport = threadnum;
    else
      maxport = souport + seg_len - 1;
    int val = 1;
    std::thread t(portScan::Action,std::ref(p_));
    t.join();
  }
  return 0;
}
