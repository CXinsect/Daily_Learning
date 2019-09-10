#include "sum.h"
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>
#include <errno.h>
int main (void) {
    void *handle;
    int (*addvec)(int ,int);
    char *error = NULL;
    handle = dlopen("./libsum.so",RTLD_LAZY);
    assert(handle != NULL);
    addvec = dlsym(handle,"sum");
    if(addvec == NULL) {
        fprintf(stderr,"%s\n",dlerror());
    }
    assert(dlerror() == NULL);
    int ret = addvec(1,2);
    printf("ret:%d\n",ret);
    assert(dlclose(handle) >= 0);
    return 0;
}