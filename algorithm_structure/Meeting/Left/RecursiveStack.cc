
//利用递归实现栈的逆序
#include <iostream>
#include <stack>

int getBottom(std::stack <int>& tmp) {
    int ret = tmp.top();
    tmp.pop();
    if(tmp.empty())
        return ret;
    else {
        int temp = getBottom(tmp);
        tmp.push(ret);
        return temp;
    }
}
void Reverse(std::stack<int> &s) {
    if(s.empty())
        return;
    else {
        int i = getBottom(s);
        Reverse(s);
        s.push(i); 
    }
}
int main (void) {
    std::stack<int> s;
    for(int i = 0;i < 5;i++) {
        s.push(i);
    }
    Reverse(s);
    while(!s.empty()) {
        std::cout << "stack: " << s.top() << std::endl;
        s.pop();
    }
    return 0;
}