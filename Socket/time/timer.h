#pragma once
#include "model.h"

#define BUFFSIZE 1024
class U_timer;
struct client_data {
    struct sockaddr_in address;
    int sockfd;
    char buf[BUFFSIZE];
    U_timer * timer;
};
//定时器类
class U_timer
{
public:
    int expire;
    void (*func)(client_data*);
    client_data * user_data;
    U_timer * prev;
    U_timer * next;
public:
    U_timer() : prev(NULL),next(NULL) {}
    ~U_timer() {}
};
//基于双向链表的定时器类
class List {
public:
    List():head(NULL),tail(NULL) {}
    ~List() {
        cout<<"aaaaaaaaaa"<<endl;
        U_timer *tmp = head;
        while(tmp) {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }
private:
    void add_timer(U_timer *,U_timer *);
public:
    void add_timer(U_timer *);
    void del_timer(U_timer *);
    void adjust_timer(U_timer *);
    void tick();
private:
        U_timer *head;
        U_timer *tail;
};

