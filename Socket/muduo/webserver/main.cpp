#include "EventLoop.cc"
#include <thread>
void threadFunc() {
    printf("pid: %d\n",getpid());
    EventLoop loop;
    loop.loop();
}
int main (void) {
    printf("main pid: %d\n",getpid());
    EventLoop loop;
    std::thread t(threadFunc);
    loop.loop();
    t.join();
    // pthread_exit(NULL);
    return 0;
}