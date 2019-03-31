#pragma once
#include "model.h"

#define BUFFSIZE 1024
class Tw_timer;

struct client_data {
    struct sockaddr_in address;
    int sockfd;
    char buf[BUFFSIZE];
    Tw_timer *timer;
};
//定时器类
class Tw_timer {
public:
    Tw_timer(int _rot,int _slot)
        :rotation(_rot),slot(_slot),prev(NULL),next(NULL){}
    Tw_timer()
        :rotation(0),slot(0),prev(NULL),next(NULL){}
public:
    //时间轮生效的轮数
    int rotation;
    //所处的槽值
    int slot;
    //回调函数
    void (*func)(client_data *);
    //用户数据
    struct client_data * user_data;
    Tw_timer *prev;
    Tw_timer *next;

};
class Time_wheel
{
public:
    Time_wheel() : cur_slot(0) {
        for(int i = 0;i< N;i++) {
            slot[i] = NULL;
        }
    }
    ~Time_wheel() {
        for(int i = 0; i < N;i++) {
            Tw_timer *tmp = slot[i];
            while(tmp) {
                slot[i] = tmp->next;
                delete tmp;
                tmp = slot[i];
            }
        }
    }
public:
    Tw_timer * add_timer(int timeout);
    void del_timer(Tw_timer *);
    //执行任务函数
    void tick();
public:
    static const int N = 60;
    static const int si = 1;
    Tw_timer *slot[N];
    int cur_slot;
};

