#include <iostream>
#include "bst.cpp"
int main()
{
    std::cout << "Hello world" << std::endl;
    BST b;
    b.add(3);
    b.add(5);
    b.add(2);
    b.add(8);
    b.add(7);
    b.add(9);
  //  b.levelOrder(b.root);
  //  b.NoCursion();
  //  b.deleteMin();
  //  b.deleteMax();
    b.deleteAny(8);
    b.preorder(b.root);
    if(b.isEmpty())
        cout<<"empty"<<endl;
    b.deleteElem(b.root);
    return 0;
}

