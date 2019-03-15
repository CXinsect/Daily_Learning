#include "seg_tree.cpp"

using namespace std;
int main()
{
    int a [] = {1,9,-5,3,2};
    seg_tree s(a,5);
    s.create_seg_tree();
    int ret =  s.query_seg_tree(0,2);
    s.set(2,4);
    ret =  s.query_seg_tree(0,2);
    s.set_section(0,2,4);
    ret = s.query_seg_tree(0,2);
    cout<<"ret: "<< ret <<endl;
    return 0;
}

