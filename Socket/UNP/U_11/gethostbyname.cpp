#include "../model.h"
#include <netdb.h>
#include <iostream>
int main(int argc, char *argv[])
{
    char *ptr, **pptr;
    char str[1024] = {0};
    struct hostent *htr = NULL;
    if (argc < 2)
    {
        cout << "参数错误" << endl;
        exit(0);
    }
    while(--argc > 0) {
        ptr = *++argv;
        if((htr = gethostbyname(ptr)) == NULL) {
            hstrerror(h_errno);
            continue;
        }
        cout<<"Offical Name: "<<htr->h_name<<endl;
        for(pptr = htr->h_aliases;*pptr != NULL;pptr++) {
            cout<<"alias: "<<*pptr<<endl;
        }
        switch(htr->h_addrtype) {
            case AF_INET:
            pptr = htr->h_addr_list;
            while(*pptr != NULL) {
                cout<<"adress: "<<inet_ntop(htr->h_addrtype,*pptr,str,sizeof(str))<<endl;
                pptr++;
            }
            break;
            default:
            cout<<"Error"<<endl;
            break;
        }
    }
}
