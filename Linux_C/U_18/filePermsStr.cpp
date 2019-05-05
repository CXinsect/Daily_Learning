#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;
#define FP_SPECIAL 1
#define SRC_SIZE sizeof("rwxrwxrwx")
#define _BSD_SOURCE
#define MYFILE "myfile"
#define MYDIR "mydir"
#define FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define DIR_PERMS (S_IRWXU | S_IRWXG | S_IRWXO)
#define UMASK_SETTING (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH)

char *filePermStr(mode_t perm,int flags) {
    static char buf[SRC_SIZE];
    snprintf(buf,SRC_SIZE,"%c%c%c%c%c%c%c%c%c",
    (perm & S_IRUSR) ? 'r' : '-',(perm & S_IWUSR) ? 'w' : '-',
    (perm & S_IXUSR) ? (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's':'x'):
                       (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S':'-'),
    (perm & S_IRGRP) ? 'r' : '-',(perm & S_IWGRP) ? 'w' : '-',
    (perm & S_IXGRP) ? (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's':'x'):
                       (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S':'-'),
    (perm & S_IROTH) ? 'r' : '-',(perm & S_IWOTH) ? 'w' : '-',
    (perm & S_IXOTH) ? (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't':'x'):
                       (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T':'-'));
    return buf;
}
int main(int argc,char * argv[]) {
    int fd;
    struct stat sb;
    mode_t u;
    umask(UMASK_SETTING);
    if((fd = open(MYFILE,O_RDWR | O_CREAT | O_EXCL,FILE_PERMS)) < 0) {
        cout<<"open err"<<endl;
        exit(0);
    }
    if(mkdir(MYDIR,DIR_PERMS) < 0) {
        cout<<"mkdir err"<<endl;
        exit(0);
    }
    u = umask(0);
    if(stat(MYFILE,&sb) == -1) {
        cout<<"stat err"<<endl;
        exit(0);
    }
    cout<<
    "Mode Wanted: "<<filePermStr(FILE_PERMS,0)<<
    " Umask: "<<filePermStr(u,0)<<
    " Reality: "<<filePermStr(sb.st_mode,0)<<endl;

    //修改特定位权限
    mode_t mode = (sb.st_mode | S_IROTH) & ~S_IRGRP;
    if(chmod(MYFILE,mode) == -1) {
        cout<<"chmod err"<<endl;
        exit(0);
    }
    cout<<"Chmod Reality: "<<filePermStr(mode,0)<<endl;
    
    if(stat(MYDIR,&sb) == -1) {
        cout<<"stat err"<<endl;
        exit(0);
    }
    
    cout<<
    "Mode Wanted: "<<filePermStr(DIR_PERMS,0)<<
    " Umask: "<<filePermStr(u,0)<<
    " Reality: "<<filePermStr(sb.st_mode,0)<<endl;
    
    if(unlink(MYFILE) == -1) {
        cout<<"unlink err"<<endl;
        exit(0);
    }
    if(rmdir(MYDIR) == -1) {
        cout<<"rmdir err"<<endl;
        exit(0);
    }
    return 0;
}