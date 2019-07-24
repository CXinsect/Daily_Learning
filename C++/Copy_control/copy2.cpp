#include <iostream>
#include <string>
#include <memory>

class A {
    public:
        A(const std::string & ptr = std::string()) :
            ps(std::shared_ptr <std::string> 
                        (new std::string (ptr))),i(0) {}
        A(const A & a) : ps(std::shared_ptr <std::string>
                        (new std::string (*a.ps))),i(0) {}
        A & operator= (const A & a);
        void print (void) {
            std::cout<< "content: "<<*ps<<std::endl;
        }
    private:
        std::shared_ptr <std::string> ps;
        int i;
};
A & A::operator= (const A & a) {
    auto temp = std::make_shared <std::string> (*a.ps);
    ps = temp;
    i = a.i;
    return *this;
}
int main (void) {
    A a("hello");
    a.print();
    A b;
    b = a;
    b.print();
    A c(b);
    c.print();
    return 0;
    return 0;
}