#include <iostream>
#include <vector>
#include <algorithm>
class Func {
    public:
        Func sorted () && {
            std::cout << "a" << std::endl;
            std::sort(a.begin(),a.end());
            return *this;
        }
        // Func sorted () const & {
        //     std::cout << "b" << std::endl;
        //     Func ret(*this);
        //     std::sort(ret.a.begin(),ret.a.end());
        //     return ret;
        // }
        Func sorted () const & {
            Func ret(*this);
            Func && r = ret.sorted();
            return ret.sorted();
            // return Func(*this).sorted();
        }
    private:
        std::vector <int> a;
};

int main (void) {
    Func f;
    return 0;
}