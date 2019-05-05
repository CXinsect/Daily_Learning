#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

static void
listFiles(const char * dirpath) {
    DIR *path;
    struct dirent * dir;
    bool isCurent;
    isCurent = strcmp(dirpath,".") == 0;
    path = opendir(dirpath);
    if(path == NULL) {
        cout<<"opendir err"<<endl;
        exit(0);
    }
    while(1) {
        errno = 0;
        dir = readdir(path);
        if(dir == NULL)
            break;
        if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0)
            continue;
        if(!isCurent)
            printf("%s/",dirpath);
        cout<<dir->d_name<<endl;
    }
    if(errno != 0) {
        cout<<"readdir err"<<endl;
        exit(0);
    }
    if(closedir(path) == -1) {
        cout<<"closedir err"<<endl;
        exit(0);
    }
}
int main (int argc,char *argv[]) {
    cout<<argc<<endl;
    if(argc < 1 || strcmp(argv[1],"--help") == 0) {
        cout<<"index err"<<endl;
        exit(0);
    }
    if(argc == 1)
        listFiles(argv[1]);
    else {
        for(argv++;*argv;argv++)
            listFiles(*argv);
    }
    return 0;
}