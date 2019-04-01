#pragma once
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#define ERR_EXIT(m) \
 do { \
    perror(m); \
    exit(EXIT_FAILURE); \
}while(0)
using namespace std;
