#include "myPool.h"

#include <iostream>
using namespace _Alloc;
char* Alloc::start_free = 0;
char* Alloc::end_free = 0;
size_t Alloc::heap_size = 0;
Alloc::obj* Alloc::free_list[__NFREELISTS] = {0};

int main (void) {
    _Alloc::Alloc a;
    a.Print(129);
    return 0;
}