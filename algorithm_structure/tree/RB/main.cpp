#include "rb.cpp"
#include <iostream>
int main()
{
    BST b;
    for(int i = 0;i < 5;i++) {
        b.add(i + 1);
    }
    b.preorder();
    b.deleteElem();
    std::cout << "Hello world" << std::endl;
    return 0;
}

