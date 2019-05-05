#include <ftw.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

static void 
usageError (const char *pName,const char *msg) {
    if(msg != NULL) 
        fprintf(stderr,"%s\n",msg);
    fprintf(stderr,"%s [-d] [-m] [-p] [filepath]\n",pName);
    exit(EXIT_FAILURE);
}
static int
dirTree(const char *pName,const struct stat *buf,int type,struct FTW *ftwb) {
    switch(buf->st_mode & S_IFMT) {
        case S_IFREG: cout<<"- "; break;
        case S_IFDIR: cout<<"d "; break;
        case S_IFCHR: cout<<"c "; break;
        case S_IFLNK: cout<<"l "; break;
        case S_IFIFO: cout<<"p "; break;
        case S_IFSOCK:cout<<"s "; break;
        case S_IFBLK: cout<<"b "; break;
        default:cout<<"Never ";   break;
    }    
    printf("%s ",(type == FTW_D) ? "D " : (type == FTW_DNR) ? "DNR " :
                 (type == FTW_F) ? "F"  : (type == FTW_DP)  ? "DP" :
                 (type == FTW_SL)? "SL" : (type == FTW_SLN) ? "SLN" :
                 (type == FTW_NS)? "NS" : " ");
    if(type != FTW_NS) 
        printf("%7ld ",(long)buf->st_ino);
    else {
        cout<<"    ";
    }
    printf("%*s",4 * ftwb->level,"");
    cout<<&pName[ftwb->base]<<endl;
    return 0;
}
int main (int argc,char *argv[]) {
    int flags,opt;
    while((opt = getopt(argc,argv,"dmp")) != -1) {
        switch(opt) {
            case 'd': flags |= FTW_DEPTH;   break;
            case 'm': flags |= FTW_MOUNT;   break;
            case 'p': flags |= FTW_PHYS;    break;
            default: usageError(argv[0],NULL);  break;
        }
    }
    cout<<optind<<endl;
    if(argc > optind + 1)
        usageError(argv[0],NULL);
    if(nftw((argc > optind) ? argv[optind] : ".",dirTree,10,flags) == -1) {
        cout<<"nftw err"<<endl;
        exit(0);
    }
    return 0;
}