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
class scoped_join {
    thread  t;
    explicit scoped_join (thread t_) :t (move(t_)) {
        if(!t.joinable()) {
            throw logic_error("No thread");
        }
    }
    ~scoped_join() {
        t.join();
    }
    scoped_join (scoped_join const & ) = delete;
    scoped_join& operator= (scoped_join const &) = delete;
};
struct func;

int main (void) {
    int i;
    scoped_join t (thread(func(i)));

    return 0;
}