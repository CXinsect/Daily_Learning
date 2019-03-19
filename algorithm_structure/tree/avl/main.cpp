#include "avl.cpp"

#include <iostream>

using namespace std;
int main()
{
    BST a;
    for(int i = 0;i < 5;i++) {
        a.add(i+1);
    }
    if(a.Is_avl_tree()) {
        cout<<"This is an Avl Tree"<<endl;
    }
    else {
        cout<<"on no"<<endl;
    }
    cout<<"size： "<<a.getSize()<<endl;
    a.deleteAny(5);
    cout<<"size： "<<a.getSize()<<endl;
    if(a.Is_avl_tree()) {
        cout<<"This is an Avl Tree"<<endl;
    }
    a.NoCursion();
    a.Delete();
    return 0;
}

