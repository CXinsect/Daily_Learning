#include "myAlloc.h"
#include <vector>
#include <iostream>

using namespace std;
int main(void) {
    
    int a[5] = {5,4,3,2,1};
    unsigned int  i;
    vector <int,t::allocator<int> > tmp(a,a+5);
    for(int i = 0;i < 5;i++) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}