#include "get_num.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
static void
getFail(const char *fname,const char *msg,const char *arg,const char *name) {
    fprintf(stderr,"%s err",fname);
    if(name != NULL) {
        fprintf(stderr,"(in %s)",name);
    }
    fprintf(stderr,"%s\n",msg);
    if(arg != NULL && *arg != '\0')
        fprintf(stderr,"    err: %s\n",arg);
    exit(EXIT_FAILURE);
}
static long
getNum(const char *fname,const char *arg,int flags,const char *name) {
    long res;
    char *endptr;
    int base;
    if(arg == NULL || *arg == '\0')
        getFail(fname,"None / Empty String",arg,name);
    base = (flags & GN_ANY_BASE) ? 0:(flags & GN_BASE_8) ? 8 :
            (flags & GN_BASE_16) ? 16 : 10;
    errno = 0;
    res = strtol(arg,&endptr,base);
    if(errno != 0) 
        getFail(fname,"strol func err",arg,name);
    if(*endptr != '\0') 
        getFail(fname,"non-numeric characters",arg,name);
    if((flags & GN_NONEG) && res < 0) 
        getFail(fname,"Negative value",arg,name);
    if((flags & GN_GT_0) && res <= 0)
        getFail(fname,"Value must be > 0",arg,name);
    return res;
}
long
getLong(const char *arg,int flags,const char *name) {
    return getNum("getLong",arg,flags,name);    
}
int
getInt(const char *arg,int flags,const char *name) {
    long res;
    res = getNum("getInt",arg,flags,name);
    if(res > INT_MAX || res < INT_MIN)
        getFail("getInt","Out Of Range",arg,name);
    return (int) res;
}