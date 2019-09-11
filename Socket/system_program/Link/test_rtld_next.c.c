#include <stdio.h>
#include <malloc.h>
#include "mymalloc.h"
int main (void) {
    void * p = memAlloc(32);
    printf("main:%p\n",p);
    memFree(p);
    return 0;
}