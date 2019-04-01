#include "model.h"
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
void read_cb(struct bufferevent *bev,void * arg) {
    char buf[1024] = {0};
    bufferevent_read(bev,buf,sizeof(buf));
    cout<<"buf: "<<buf<<endl;
}
void write_cb(struct bufferevent * bev,void *arg) {
    cout<<"I am Write_cb"<<endl;
}
void event_cb(struct bufferevent * bev,short events,void * arg) {
    if(events & BEV_EVENT_EOF) {
        cout<<"connect failed"<<endl;
    }
    else if(events & BEV_EVENT_ERROR) {
        cout<<"Other error"<<endl;
    }
    else if(events & BEV_EVENT_CONNECTED) {
        cout<<"Connected"<<endl;
        return;
    }
    bufferevent_free(bev);
}
void send_cb(evutil_socket_t fd,short what,void * arg) {
    char buf[1024] = {0};
    struct bufferevent * bev = (struct bufferevent *)arg;
    cout<<"Please enter the data"<<endl;
    read(fd,buf,sizeof(buf));
    bufferevent_write(bev,buf,sizeof(buf));
}
int main(int argc,char * argv[])
{
    if(argc < 3) {
        cout<<"Please Enter like [./a.out] [Port] [Ip]"<<endl;
        return -1;
    }
    struct event_base * base = event_base_new();
    struct bufferevent * bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
    struct sockaddr_in serv;
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(argv[1]));
    evutil_inet_pton(AF_INET,argv[2],&serv.sin_addr.s_addr);
    bufferevent_socket_connect(bev,(struct sockaddr *)&serv,sizeof(serv));
    bufferevent_setcb(bev,read_cb,write_cb,event_cb,NULL);
    bufferevent_enable(bev,EV_READ | EV_PERSIST);
    //创建事件
    struct event *ev = event_new(base,STDIN_FILENO,
                                       EV_READ | EV_PERSIST,send_cb,bev);
    //将事件添加到事件处理框架
    event_add(ev,NULL);
    //循环事件
    event_base_dispatch(base);
    //删除事件
    event_base_free(base);
    return 0;
}

