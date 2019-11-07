#include "myPool.h"

void* _Alloc::Alloc::allocate(size_t n) {
    obj** myFreeList = 0;
    obj* result = 0;
    if(n > (size_t)__MAX_BYTES) {
        void *p = malloc(n*sizeof(int));
        if(0 == p) {
            std::cout << "out of memory" << std::endl;
            exit(1);
        }
        std::cout << std::endl;
        return p;
    }
    myFreeList = free_list + FREELIST_INDEX(n);
    result = *myFreeList;
    if(0 == result) {
        void* r = refill(ROUND_UP(n));
        return r;
    }
    *myFreeList = result->next;
    return result;
}
void _Alloc::Alloc::delallocate(void* ptr,size_t n) {
    obj* q = (obj*)ptr;
    obj** myFreeList;
    if(n > (size_t)__MAX_BYTES) {
        std::cout << "free" << std::endl;
        free(ptr);
        return;
    }
    myFreeList = free_list + FREELIST_INDEX(n);
    //调整自由链表，将节点回收至自由链表的头部
    q->next = *myFreeList;
    *myFreeList = q;
}
void* _Alloc::Alloc::refill(size_t n) {
    int nobjs = __STANDERNUM;
    char* chunk = chunk_alloc(n,nobjs);
    obj** MyFreeList;
    obj* result;
    obj* currentObj;
    obj* nextObj;
    int i;
    if(1 == nobjs) return chunk;
    MyFreeList = free_list + FREELIST_INDEX(n);
    result = (obj*) chunk;
    *MyFreeList = nextObj = (obj*)(chunk + n);
    for(i = 1;;i++) {
        currentObj = nextObj;
        nextObj = (obj*)((char* )nextObj + n);
        if(nobjs-1 == i) {
            currentObj->next = NULL;
            return result;
        }
        else {
            currentObj->next = nextObj;
        }
    }
    return result;
}
char* _Alloc::Alloc::chunk_alloc(size_t size,int& nobjs) {
    char* result;
    //分配的总字节数
    size_t totalBytes = size * nobjs;
    size_t bytesLeft = end_free - start_free;
    //满足标准分配要求
    if(bytesLeft >= totalBytes) {
        result = start_free;
        start_free += totalBytes;
        return result;
    }
    //只能够满足一个以上标准以下
    else if(bytesLeft >= size) {
        //调整分配个数
        nobjs = bytesLeft/size;
        totalBytes = size * nobjs;
        result = start_free;
        start_free += totalBytes;
        return result; 
    }
    //一个都满足不了，需要重新在堆上分配空间
    else {
        size_t byteGet = 2*totalBytes + ROUND_UP(heap_size >> 4);
        if(bytesLeft > 0) {
            obj** myFreeList = free_list + FREELIST_INDEX(bytesLeft);
            ((obj* )start_free)->next = *myFreeList;
            *myFreeList = (obj*)start_free;
        }
        start_free = (char*)malloc(byteGet);
        //内存已经山穷水尽,想办法从相邻的链表中获取空闲空间
        if(0 == start_free) {
            int i;
            obj** myFreeList;
            obj* p;
            for(int i = size;i < __MAX_BYTES; i += __ALIGN) {
                myFreeList = free_list + FREELIST_INDEX(i);
                p = *myFreeList;
                *myFreeList = p->next;
                start_free = (char*)p;
                end_free = start_free + i;
                return chunk_alloc(size,nobjs);
            }
            //内存已经用尽，最后的尝试也已失效
            end_free = 0;
        }
        heap_size += byteGet;
        end_free = start_free + byteGet;
        return chunk_alloc(size,nobjs);
    }
}