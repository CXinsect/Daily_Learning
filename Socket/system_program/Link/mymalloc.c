#ifdef RUNTIME
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include "mymalloc.h"
void *malloc(size_t size) {
    void*(*mallocp)(size_t size);
    char *error;
    // printf("ret:D%d\n",size);
    mallocp = dlsym(RTLD_NEXT,"malloc");
    // assert(dlerror() == NULL);
    printf("hello\n");

    char *ptr = mallocp(size);
    // printf("ret:D%d\n",size);
    return ptr;
}
void free(void *ptr) {
    // assert(ptr != NULL);
    char *error;
    void (*freep)(void *ptr);
    freep = dlsym(RTLD_NEXT,"free");
    // assert(dlerror() == NULL);
    freep(ptr);
    // printf("free:D%p\n",ptr);
}
void *memAlloc (size_t size) {
    void *ptr = malloc(size);
    printf("ret:%d : %p\n",size,ptr);
    return ptr;
}
void memFree(void *ptr) {
    printf("free ret1: %p\n",ptr);
    free(ptr);
    printf("free ret: %p\n",ptr);
}
#endif