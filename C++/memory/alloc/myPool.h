#include <cstdlib>
#include <new>
#include <iostream>

namespace _Alloc {

enum {__ALIGN = 8 };
enum {__MAX_BYTES = 128 };
enum {__NFREELISTS = __MAX_BYTES/__ALIGN };
enum {__STANDERNUM = 20 };
class Alloc {
    private:
        union obj {
            union obj* next;
            char client_data[1];
        };
        static obj* free_list[__NFREELISTS];
        static size_t FREELIST_INDEX(size_t bytes) {
            return (((bytes)+__ALIGN-1)/__ALIGN-1);
        }
        static void* refill(size_t n);
        static char* chunk_alloc(size_t size, int &nobjs);
        static char* start_free;
        static char* end_free;
        static size_t heap_size;
        static void* allocate(size_t n);
        static void delallocate(void* p,size_t n);
        static void* reallocate(void* p ,size_t old_size,size_t new_size);
    public:  
        Alloc() = default;
        //上调至８的边界
        static size_t ROUND_UP(size_t bytes) {
            return (((bytes) + __ALIGN-1) &~(__ALIGN-1));
        } 
        void Print(int n) {
            int* p = (int*)allocate(n);
            for(int i = 0;i < n;i++) 
                p[i] = i+1;
            for(int i = 0;i < n;i++) {
                printf("%d ",p[i]);
            }
            delallocate(p,n);
            std::cout << std::endl;
        }
};
// char* Alloc::start_free = 0;
// char* Alloc::end_free = 0;
// size_t Alloc::heap_size = 0;
// Alloc::obj* Alloc::free_list[__NFREELISTS] = {0};
}