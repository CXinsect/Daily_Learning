#include <iostream>
#include <cstdlib>
#include <cassert>
#include <ext/pool_allocator.h>
class Allocator {
    private:
        struct obj {
            struct obj* next;
        };
    public:
        void* allocate(size_t size);
        void delallocate(void *phead,size_t size);
    private:
        obj* head = nullptr;
        const int SIZE = 5;
};
void* Allocator::allocate(size_t size) {
    assert(size > 0);
    obj* p;
    if(!head) {
        size_t _chunk = SIZE * size;
        head = p = (obj*)malloc(_chunk);

        for(int i = 0;i < SIZE; i++) {
            p->next = (obj*)((char*)p + size);
            p = p->next;    
        }

        p->next = nullptr;
    }
    p = head;
    head = head->next;
    std::cout << "allocate executed" << std::endl;
    return p;
}
void Allocator::delallocate(void *phead,size_t size) {
   
    ((obj*)phead)->next = head;
    head = (obj*)phead;
    std::cout << "delallocate executed" << std::endl;
}

class A {
    public:
        static void* operator new (size_t size) {
            return Myalloc_.allocate(size);
        }
        static void operator delete(void* ptr,size_t size) {
             return Myalloc_.delallocate(ptr,0);
        }
        static void operator delete[](void *ptr,size_t size);
        A() {
            std::cout << "hello,Linux" << std::endl;
        }
    public:
        static Allocator Myalloc_;
};
Allocator A::Myalloc_;
void A::operator delete[](void *ptr,size_t size) {
     return Myalloc_.delallocate(ptr,0);
}

int main (void) {
    // A * a = new A[5];
    // delete [] a;
    std::cout << sizeof(__gnu_cxx::__pool_alloc<int>) << std::endl;
    return 0;
}