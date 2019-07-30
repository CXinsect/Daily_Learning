#include <iostream>
#include <vector>
#include <memory> 

class Base {
    public:
        virtual void func () {
            std::cout << "base" << std::endl;
        }
};

class B1 : public Base {
    public:
        void func () {
            std::cout << "B1" << std::endl;
        }
};

int main () {
    std::vector <std::shared_ptr <Base>> v;
    v.push_back(std::make_shared<Base>());
    v.push_back(std::make_shared<B1>());
    v.front()->func();
    return 0;
}
