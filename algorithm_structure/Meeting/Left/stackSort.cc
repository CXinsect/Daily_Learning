//用一个栈实现另一个栈的排序
#include <iostream>
#include <stack>
int main (void) {
    std::stack<int> s;
    s.push(5);
    s.push(2);
    s.push(4);
    s.push(3);
    s.push(1);
    std::stack<int>s1;
    std::cout << "aa" << std::endl;
    while(!s.empty()) {
        int cur = s.top();
        s.pop();
        while(!s1.empty() && cur > s1.top()) {
            int tmp = s1.top();
            s1.pop();
            s.push(tmp);
        }
        s1.push(cur);
    }
    std::cout << "bb" << std::endl;
    while(!s1.empty()) {
        std::cout << "stack: " << s1.top() << std::endl;
        s1.pop();
    }
    return 0;
}