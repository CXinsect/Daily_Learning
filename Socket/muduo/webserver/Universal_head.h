#ifndef UNIVERSAL__HEAD__
#define UNIVERSAL__HEAD__
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h> /* See NOTES */
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <assert.h>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/implicit_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class TcpConnection;
class Buffer;
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallBack;
// typedef std::function<void (const TcpConnectionPtr&,
//                             const char*buf,int len)> MessageCallBack;
typedef void (*MessageCallBack)(const TcpConnectionPtr&, Buffer* buf);

typedef std::function<void(const TcpConnectionPtr&)> CloseCallBack;
typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn) {
  std::cout << "Connected" << std::endl;
  // do not call conn->forceClose(), because some users want to register message
  // callback only.
}

void defaultMessageCallback(const TcpConnectionPtr&, Buffer* buf);

typedef std::function<void()> CallBack;

class SignalPipe {
 public:
  SignalPipe() { ::signal(SIGPIPE, SIG_IGN); }
};
#endif