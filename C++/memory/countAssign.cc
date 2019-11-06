#include <iostream>
#include <list>
#include <ext/pool_allocator.h>

int countNew = 0;
int numNew = 0;
template <typename T>
using listPool = std::list<T,__gnu_cxx::__pool_alloc<T>>;

static void* Mymalloc (size_t size) {
    return malloc(size);
}
static void Myfree (void* ptr) {
    return free(ptr);
}
void* operator new(size_t size) { 
    countNew += size;
    numNew++;  
    return Mymalloc(size);
}
void operator delete(void* ptr,size_t size) {
    return Myfree(ptr);
}
int main (void) {
    std::list<int> l;
    for(int i = 0;i < 100000;i++) {
        l.push_back(i);
    }
    std::cout << "countNew" << countNew << std::endl;
    std::cout << "numNew" << numNew << std::endl;
    countNew = 0;
    numNew = 0;
    listPool<int>l2;
    for(int i = 0;i < 100000;i++) {
        l2.push_back(i);
    }
    std::cout << "countNew" << countNew << std::endl;
    std::cout << "numNew" << numNew << std::endl;
}

