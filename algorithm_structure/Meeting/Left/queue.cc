
//利用两个栈实现队列
#include <iostream>
#include <stack>

class A {
    public:
        void add(int num) {
            s.push(num);
        }
        void poll() {
            while(!s.empty() && s1.empty()) {
                s1.push(s.top());
                s.pop();
            }
            s1.pop();
        }
        int peek() {
            while(!s.empty() && s1.empty()) {
                s1.push(s.top());
                s.pop();
            }
            return s1.top();
        }
    private:
        std::stack<int> s;
        std::stack<int> s1;
};

int main (void) {
    A a;
    for(int i = 5;i >0;i--) {
        a.add(i);
    }
    std::cout << "peek: " << a.peek() << std::endl;
    a.poll();
    std::cout << "pop" << std::endl;
    std::cout << "peek: " << a.peek() << std::endl;    
    return 0;
}