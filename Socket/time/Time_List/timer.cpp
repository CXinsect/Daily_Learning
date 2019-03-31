#include "timer.h"

void List::add_timer(U_timer *timer,U_timer *last_node) {
    if(!timer) {
        cout<<"The Timer is Empty [func] add_timer"<<endl;
        return;
    }
    U_timer * prev = last_node;
    U_timer * tmp = prev->next;
    //遍历当前节点的后面节点，如果比当前节点的超时事件长，
    //则将目标节点插入当前节点的前面
    while(tmp) {
        if(timer->expire < tmp->expire) {
            prev->next = timer;
            timer->next = tmp;
            tmp->prev = timer;
            timer->prev = prev;
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    //如果当前链表的超时事件都比目标定时器的超时事件长，
    //则将目标定时器插入当前链表的尾节点
    if(!tmp) {
        prev->next = timer;
        timer->prev = prev;
        timer->next = NULL;
        tail = timer;
    }
}
void List::add_timer(U_timer *timer) {
    if(!timer) {
        return;
    }
    //如果链表为空
    if(!head){
        head = tail = timer;
        return;
    }
    //如果链表不为空
    if(timer->expire < head->expire) {
        timer->next = head;
        head->prev = timer;
        head = timer;
        return;
    }
    add_timer(timer,head);
}
void List::del_timer(U_timer *timer) {
    if(!timer) {
        return;
    }
    //链表中定时期只有一个
    if(head == timer && tail == timer) {
        delete timer;
        head = NULL;
        tail = NULL;
        return;
    }
    //链表中定时器有两个且目标定时期是头结点
    if(timer == head) {
        head = head->next;
        head->prev = NULL;
        delete timer;
        return;
    }
    //链表中定时期有两个且目标定时期是尾节点
    if(timer == tail) {
        tail = tail->prev;
        tail->next = NULL;
        delete timer;
        return;
    }
    //目标定时期处于中间部分
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    delete timer;
}
void List::adjust_timer(U_timer *timer) {
    if(!timer) {
        return;
    }
    U_timer *tmp = timer->next;
    //如果目标定时器处于链表尾部或者当前定时器小于下一个定时器的超时时间
    if((!tmp)  ||  (timer->expire < tmp->expire)) {
        return;
    }
    //如果目标定时器超时时间比头结点的超时时间要小，
    //则将该定时器插入头结点之前
    if(timer == head) {
        head = head->next;
        head->prev = NULL;
        timer->next = NULL;
        add_timer(timer,head);
    }
    //如果是中间部分
    else {
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        add_timer(timer,timer->next);
    }
}
//处理超时任务
void List::tick() {
    if(!head) {
        return;
    }
    time_t cur = time(NULL);
    U_timer *tmp = head;
    while(tmp) {
        if(cur < tmp->expire)
            break;
        //超时则处理超时任务并删除相应的定时器
        tmp->func(tmp->user_data);
        head = tmp->next;
        if(head)
            head->prev = NULL;
        delete tmp;
        tmp = head;
    }
}
