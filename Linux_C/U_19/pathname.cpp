#include <libgen.h>
#include <string.h>
#include <iostream>
using namespace std;

int main (int argc,char * argv[]) {
    char * t1,*t2;
    for(int i = 1; i < argc;i++) {
        if((t1 = strdup(argv[i])) == NULL) {
            cout<<"strdup err"<<endl;
            exit(0);
        }
         if((t2 = strdup(argv[i])) == NULL) {
            cout<<"strdup err"<<endl;
            exit(0);
        }
        //cout<<argv[i]<<"==>>"<<dirname(t1)<<basename(t2)<<endl;
        char buf[1024] = {0};
        snprintf(buf,sizeof(buf),"%s/%s",dirname(t1),basename(t2));
        cout<<"buf: "<<buf<<endl;
        free(t1);
        free(t2);
    }
    exit(EXIT_SUCCESS);
}