#include "sig_functions.h"

void printSigset(FILE *of,const char *prefix,const sigset_t *sigset) {
    int sig,cnt;
    cnt = 0;
    for(sig = 1;sig< NSIG;sig++) {
        if(sigismember(sigset,sig)) {
            cnt++;
            fprintf(of,"%s%s(%s)",prefix,sig,strsignal(sig));
        }
     }
     if(cnt == 0) 
        fprintf(of,"%s < empty signal set>\n",prefix);
}
int printSigMask(FILE *of,const char * msg) {
    sigset_t curMask;
    printf("length:%d\n",strlen(msg));
    if(msg != NULL) {
        fprintf(of,"%s",msg);
    }
    if(sigprocmask(SIG_BLOCK,NULL,&curMask) == -1)
        return -1;
    printSigset(of,"\t\t",&curMask);
    return 0;
}
int printPendinfSigs(FILE *of,const char *msg) {
    sigset_t pendings;
    if(msg != NULL)
        fprintf(of,"%s",msg);
    if(sigpending(&pendings) == -1) 
        return -1;
    printSigset(of,"\t\t",&pendings);
    return 0;
}
