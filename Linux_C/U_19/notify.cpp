#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#define BUF_SIZE (5 * (sizeof(struct inotify_event) + NAME_MAX +1))

using namespace std;
static void InotifyEvents (struct inotify_event *event) {
    printf("wd = %2d ",event->wd);

    if(event->cookie > 0)
        printf("cookie = %4d ",event->cookie);
    cout<<"mask: ";
    if(event->mask & IN_ACCESS) cout<<"IN_ACCESS";
    if(event->mask & IN_OPEN ) cout<<"IN_OPEN ";
    if(event->mask & IN_MOVED_FROM) cout<<"IN_MOVED_FROM";
    if(event->mask & IN_MOVED_TO) cout<<"IN_MOVED_TO";
    if(event->mask & IN_MODIFY) cout<<" IN_MODIFY";
    cout<<endl;
    if(event->len > 0) 
        cout<<"     Name: "<<event->name<<endl;
}

int main (int argc,char * argv[]) {
    int nfd,wd;
    char buf[BUF_SIZE] = {0};
    ssize_t numBytes;
    char * p;
    struct inotify_event *event;
    if(argc < 2 || strcmp(argv[1],"--help") == 0) {
        cout<<"index err"<<endl;
        exit(0);
    }
    if((nfd = inotify_init()) < 0) {
        cout<<"inotify_init err"<<endl;
        exit(0);
    }
    for(int i = 0;i < argc;i++) {
        if((wd = inotify_add_watch(nfd,argv[i],IN_ALL_EVENTS)) < 0) {
            cout<<"inotify_add_watch err"<<endl;
            exit(0);
        }
        cout<<"Watching "<<argv[i]<<" wd= "<<wd<<endl;
    }
    while(1) {
        numBytes = read(nfd,buf,sizeof(buf));
        if(numBytes == 0) {
            cout<<"No Events"<<endl;
            continue;
        }
        else if(numBytes < 0) {
            cout<<"read err"<<endl;
            exit(EXIT_FAILURE);
        }
        else {
            p = buf;
            cout<<"NumBytes: "<<numBytes<<endl;
            while(p < buf+numBytes) {
                event = (struct inotify_event *)p;
                InotifyEvents(event);
                p += sizeof(struct inotify_event) + event->len;
            }
        }
    }
    return 0;
}
