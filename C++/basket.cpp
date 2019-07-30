#include <iostream>
#include <set>
#include <memory>
#include <algorithm>
class Base {
    public:
        Base() {}
        Base (int a_) : a(a_) {}
        int getA () { return a; }
        void setA(int a_) {
            a = a_;
        }
    private:
        int a;
};
class Basket {
    public:
        Basket() {}
        void add_item(const std::shared_ptr<Base> &s ) {
             items.insert(s);
        }
        void getNumber() {
            int sum = 0;
            for(auto iter = items.cbegin();iter != items.cend();
                iter = items.upper_bound(*iter)) {
                    std::cout << "count: " << items.count(*iter) << std::endl;
                }
        }
    private:
        static bool compare(const std::shared_ptr<Base> &l,const std::shared_ptr<Base> &r) {
            return l->getA() < r->getA();
        }
         std::multiset<std::shared_ptr<Base>,decltype(compare)*> items{compare};
};
int main (void) {
    Basket b1;
    Base a(1);
    auto p = std::make_shared <Base>(a);
    b1.add_item(p);
    a.setA(2);
    p = std::make_shared <Base> (a);
    b1.add_item(p);
    Base b(1);
    p = std::make_shared<Base>(b);
    b1.add_item(p);
    b.setA(2);
    p = std::make_shared<Base>(b);
    b1.add_item(p);
    b.setA(3);
    p = std::make_shared<Base>(b);
    b1.add_item(p);
    b.setA(3);
    p = std::make_shared<Base>(b);
    b1.add_item(p);
    b1.getNumber();
    return 0;
}