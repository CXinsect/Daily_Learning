#include <iostream>
#include <memory>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>
using namespace std;

class Uio {
    public:
        Uio (const string & name_,int x_,short y_) :
                   name(name_) {} 
        void Read() {
            int fd;
            fd = open(name.c_str(),O_RDONLY);
            if(fd < 0) {
                cout << "Open Err" << endl;
                exit(0);
            }
            unique_ptr <struct iovec []> iov (new struct iovec [3]);
            char x;
            int y;
            iov[0].iov_base = &x;
            iov[0].iov_len = sizeof(x);
            int numRead;
            int numRequire = 0;
            numRequire += sizeof(x);
            iov[1].iov_base = &y;
            iov[1].iov_len = sizeof(y);
            numRequire += sizeof(y);
            
            numRead = readv(fd,iov.get(),2);
            if(numRead == -1) {
                cout << "readv err" << endl;
                exit(0);
            }   
            if(numRead != numRequire) {
                cout << "Read fewer than Requirement" << endl;
            }
            cout << numRead << " : " << numRequire << endl; 
            char * p = static_cast <char *>(&x);
            cout << *(p + 1) << endl;
            close(fd);     
        }
    private:
        string name;
};
int main (int argc,char * argv[]){
    string str = argv[1];
    int x;
    short y;
    cin >> x >> y;
    Uio u(str,x,y);
    u.Read();
    return 0;
}