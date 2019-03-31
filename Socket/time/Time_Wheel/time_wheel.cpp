#include "time_wheel.h"

Tw_timer *Time_wheel::add_timer(int timeout) {
    if(timeout < 0)
        return NULL;
    int ticks = 0;
    if(timeout < si)
        ticks = 1;
    else
        ticks = timeout / si;
    int rotation = ticks / N;
    int ts = (cur_slot + (ticks % N)) % N;
    Tw_timer *timer = new Tw_timer(rotation,ts);
    if(!slot[ts]) {
        slot[ts] = timer;
    }
    else {
        timer->next = slot[ts];
        slot[ts]->prev = timer;
        slot[ts] = timer;
    }
    return timer;
}
void Time_wheel::del_timer(Tw_timer * timer) {
    if(!timer)
        return;
    int ts = timer->slot;
    //删除的定时器是槽的头结点
    if(timer == slot[ts]) {
        slot[ts] = slot[ts]->next;
        if(slot[ts]) {
            slot[ts]->prev = NULL;
        }
        delete timer;
    }
    else {
            if(timer->next) {
                timer->prev->next = timer->next;
                timer->next->prev = timer->prev;
            }
            delete timer;
    }
}
void Time_wheel::tick() {
    Tw_timer *tmp = slot[cur_slot];
    while(tmp) {
        if(tmp->rotation > 0) {
            tmp->rotation--;
            tmp = tmp->next;
        }
        else {
            tmp->func(tmp->user_data);
            if(tmp == slot[cur_slot]) {
                slot[cur_slot] = tmp->next;
                if(slot[cur_slot]) {
                    slot[cur_slot]->prev = NULL;
                }
                delete tmp;
                tmp = slot[cur_slot];
            }
            else {
                if(tmp->next) {
                    tmp->prev->next = tmp->next;
                    tmp->next->prev = tmp->prev;
                }
                Tw_timer * tmp2 = tmp->next;
                delete tmp;
                tmp = tmp2;
            }
        }
    }
    cur_slot = ++cur_slot % N;
}

