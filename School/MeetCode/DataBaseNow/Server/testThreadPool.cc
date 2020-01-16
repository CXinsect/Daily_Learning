#include "threadpool.h"

struct A : public Task {
    public:
        A() : a(0) {}
        ~A() {}
        void Run() override
         {
            for(int i = 0;i < 10000;i++) 
                a++;
            cout << a << endl;
        }
        // void print() { cout << a << endl; }
    private:
        int a;
};

int main (void) {
    ThreadPool* t = new ThreadPool(8);
    t->Start();
    for(int i = 0;i <8;i++)
        t->addTask(new A());
    
  
    t->Stop();
    delete t;
    // A a;
}