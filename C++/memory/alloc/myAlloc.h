#ifndef __ALLOCATOR_H_
#define __ALLOCATOR_H_

#include <new>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <climits>

namespace t {
    template <typename T>
    inline T* _allocate(ptrdiff_t size, T*) {
        T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
        if(0 == tmp) {
            std::cerr << "out of memory" << std::endl;
            exit(0);
        }
        return tmp;
    }
    template <typename T> 
    inline void _deallocate(T* buffer) {
        ::operator delete(buffer);
    }
    template <typename T1,typename T2>
    inline void _construct (T1* p,const T2& value) {
        new(p) T1(value);
    }
    template <typename T>
    inline void _destory (T* ptr) {
        ptr->~T();
    }
    template <typename T> 
    class allocator {
        public:
            typedef T value_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;

            pointer allocate (size_type n,const void* hint=0) {
                return _allocate((difference_type)n,(pointer)0);
            }

            void deallocate (pointer p,size_type n) {
                _deallocate(p);
            }

            void construct (pointer p,size_type& value) {
                _construct(p,value);
            }

            void destroy (pointer p) {
                _destory(p);
            }

            size_t max_size() const {
                return size_type(UINT64_MAX/sizeof(T));
            }
    };

}
#endif