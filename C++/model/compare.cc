#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>

template <typename T,typename F = std::less<T> >
class A {
    public:
        int compare(const T &v1, const T &v2, F f = F()) {
            if(f(v1,v2)) return -1;
            if(f(v2,v1)) return 1;
            return 0;
        }
};

class B {
    public:
        B(std::ostream &s = std::cerr) :os(s) { }
    template <typename T> void operator()(T *p) const {
        os << "delete func" << std::endl; delete p;
    }
    private:
        std::ostream &os;
};
int main (void) {
    A<int> a;
    if(a.compare(2,1) == 1) 
        std::cout << "Hello,Linux" << std::endl;
    else 
        std::cout << "Hello,Windows" << std::endl;
    std::unique_ptr<int,B>(new int(12),B());
    return 0;
}