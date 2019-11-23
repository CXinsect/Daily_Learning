#include <iostream>
#include <thread>
#include "../EventLoop.cc"

void threadFunc() {
    EventLoop loop;
    loop.loop();
}
void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    ::poll(NULL,0,3*1000);
    looping_ = false;
}
int main (void) {
    EventLoop loop;
    thread t(threadFunc);
    t.join();
    return 0;
}