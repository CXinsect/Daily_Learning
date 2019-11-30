#include <iostream>

int main (void)  {
    int a = 1;
    int *p = &a;
    *p = 2;
    p = NULL;
    std::cout << a << std::endl;
}