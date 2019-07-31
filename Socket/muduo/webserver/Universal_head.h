#ifndef UNIVERSAL__HEAD__
#define UNIVERSAL__HEAD__
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <sys/uio.h>

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <assert.h>
#include <boost/noncopyable.hpp>
#include <boost/implicit_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

class TcpConnection;
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void ( TcpConnectionPtr&)> ConnectionCallBack;
// typedef std::function<void (const TcpConnectionPtr&,
        //                             const char*buf,int len)> MessageCallBack;
typedef void (*MessageCallBack)(const TcpConnectionPtr&,
                                const char * buf,
                                int n);

typedef std::function<void (const TcpConnectionPtr&)> CloseCallBack;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;

#endif