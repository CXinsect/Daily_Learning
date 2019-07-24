#include <iostream>
#include <thread>
using namespace std;
struct func {
    int & i;
    func(int & i_) : i(i_) {}
    void operator() () {
        for (unsigned j = 0;j < 10000;++j) {
            do_something(j);
        }
    }
    public:
        void do_something(int i) {
            cout<<"i: "<<i<<endl;
        }
};
class thread_guard {
        thread  &t;
    public:
        explicit thread_guard (thread & t_): t(t_) {}
        ~thread_guard() {
            if(t.joinable()) {
                t.join();
            }
        }
        thread_guard (thread_guard const&) = delete;
        thread_guard & operator=(thread_guard const &) = delete;
};
int main (void) {
    int local = 0;
    func my_func (local);
    thread t (my_func);
    thread_guard g(t);
}