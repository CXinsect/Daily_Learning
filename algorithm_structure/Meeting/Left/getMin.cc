//获取栈中的最小值
#include <iostream>
#include <stack>

int main (void) {
    std::stack <int> s;
    std::stack <int> help;
    for(int i = 1;i < 5;i++) {
        s.push(i);
        if(!help.empty()) {
            if(i <= help.top()) {
                help.pop();
                help.push(i);
                continue;
            }
        } else {
            help.push(i);
        }
    }
    std::cout << "Min: " << help.top() << std::endl;
    return 0;
}
