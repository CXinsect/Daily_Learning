#include "heap.cpp"
#include <iostream>
using namespace std;
int main()
{
    heap h(1000);
    int a[5] = {0};
    for(int i = 0;i<5;i++) {
        a[i] = i+1;
        h.add(a[i]);
    }
    h.print();
    for(int i = 0;i<5;i++) {
        a[i] = h.exMax();
    }
    for(int i = 0;i<5;i++) {
        cout<<a[i]<<" ";
    }
    cout<<endl;
    return 0;
}

