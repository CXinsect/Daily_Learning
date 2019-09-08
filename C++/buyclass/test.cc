#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main (void) {
    // using line_no = std::vector <int>::size_type;
    // std::vector<int> v;
    // line_no t = 1;
    // std::cout << t << std::endl; 
    // return 0;
    for(int i = 0;i < 3;i++) {
        fork();
        printf(">\n");
    }
    while(waitpid(-1,NULL,WNOHANG) > 0);
}