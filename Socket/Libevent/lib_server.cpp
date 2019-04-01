#include "model.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
void read_cb (struct bufferevent * bev,void *arg) {
    cout<<"T am read_cb"<<endl;
    char buf[1024] = {0};
    bufferevent_read(bev,buf,sizeof(buf));
    cout<<"recvbuf: "<<buf<<endl;
    const char * ptr = "roger that";
    bufferevent_write(bev,ptr,strlen(ptr) + 1);
    return;
}
void write_cb(struct bufferevent *bev,void *arg) {
    cout <<"I am write_cb"<<endl;
}
void event_cb(struct bufferevent * bev,short events,void *arg) {
    cout<<"I am events_cb"<<endl;
    if(events & BEV_EVENT_EOF) {
        cout<<"Connect failed"<<endl;
    }
    else if(events & BEV_EVENT_ERROR) {
        cout<<"Other Error"<<endl;
    }
    bufferevent_free(bev);
}
void listen_cb(struct evconnlistener *lev,
               evutil_socket_t fd,
               struct sockaddr*addr,
               int len,
               void *ptr) {
    cout<<"New Student"<<endl;
    struct event_base *base = (struct event_base *)ptr;
    struct bufferevent * bev = NULL;
    bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev,read_cb,write_cb,event_cb,NULL);
    bufferevent_enable(bev,EV_READ | EV_PERSIST);
}
int main(int argc, char * argv[])
{
    if(argc < 2) {
        cout<<"Please enter like [./a.out] [port]"<<endl;
        return -1;
    }
     struct sockaddr_in serv;
    memset(&serv,0,sizeof(serv));
    serv.sin_family = AF_INET;
    int port = atoi(argv[1]);
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;
    //创建事件处理框架
    struct event_base * base = event_base_new();
    //创建链接监听器
    struct evconnlistener * lev = evconnlistener_new_bind(base,listen_cb,base,
                                                          BEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,SOMAXCONN,
                                                          (struct sockaddr*)&serv,sizeof(serv));
    //事件循环
    event_base_dispatch(base);
    //销毁链接监听器
    evconnlistener_free(lev);
    //箱销毁事件框架
    event_base_free(base);
    return 0;
}

