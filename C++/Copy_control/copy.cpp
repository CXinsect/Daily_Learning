#include <iostream>
#include <string>
class A {
    public:
        A(const std::string & ptr = std::string()): 
            ps(new std::string (ptr)),i(0) { }
        // A(const A & p) :
        //     ps(new std::string(*p.ps)), i(0) {}
        A & operator =(const A &);
        ~A() {
            delete ps;
        }
        void print (void) {
            std::cout<< "content: "<<*ps<<std::endl;
        }
    private:
        std::string * ps;
        int i;
};
A & A::operator= (const A & a) {
    auto p = new std::string (*a.ps);
    delete ps;
    ps = p;
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
}