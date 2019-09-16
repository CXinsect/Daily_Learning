#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
template <typename T>

class A {
    public:
        typedef T value_type;
        typedef typename std::vector<T>::size_type size_type;
        // A() = default;
        A() :data(new std::vector<T>) { }
        A(std::initializer_list<T> il) :data (std::make_shared<std::vector<T>>(il)){ }
        size_type size () const { return data->size(); }
        bool empty() const { return data->empty(); }
        void push_back(const T &t) { data->push_back(t); }
        void push_back(const T && t) { data->push_back(std::move(t)); }
    private:
        std::shared_ptr <std::vector<T>> data;
};

int main (void) {
    A <int> a = {1,2,3};
    std::cout << a.size() << std::endl;
    return 0; 
}