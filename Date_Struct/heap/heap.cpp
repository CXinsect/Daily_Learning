#include "heap.h"
#include <iostream>

using namespace std;
void heap::SiftUp(int index) {
    if(index < 0) {
        cout<<"The heap is Empty"<<endl;
        return;
    }
    else {
        int t = parent(index);
        while(index > 0 && a.get(index) > a.get(t)) {
            a.Swap(index,t);
            index = t;
            t = parent(index);
        }
    }
}
int heap::exMax() {
    int ret = getMax();
    cout<<"heap: "<<ret<<endl;
    a.Swap(0,a.getSize() - 1);
    cout<<"capicaty: "<<a.getCapicaty()<<"size: "<<a.getSize()<<endl;

    a.deleteTail();
    SiftDown(0);
    return ret;
}
void heap::SiftDown(int index) {
    if(index < 0 || index > a.getSize()) {
        cout<<"The index is Wrong func: SiftDown"<<endl;
        return;
    }
    else {
        while(leftChild(index) < a.getSize()) {
            int t = leftChild(index);
            while(t+1 < a.getSize() &&
                  a.get(t+1) > a.get(t))
                t++;
            if(a.get(index) > a.get(t))
                break;
            a.Swap(index,t);
            index = t;
        }
    }
}
void heap::add(int el) {
        a.addTail(el);
        cout<<"size: "<<a.getSize()<<endl;
        SiftUp(a.getSize()-1);
}
void heap:: print() {
 a.Print();   
}
void heap::Free() {
    if(a.arg != NULL)
        delete [] a.arg;
}
