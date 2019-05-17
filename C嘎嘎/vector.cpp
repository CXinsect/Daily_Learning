#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

void work(int i) {
    cout<<i<<endl;
}
int main (void) {
    vector <thread>  threads;
    for (unsigned i = 0;i < 2;i++) {
        threads.push_back(thread(work,i));
    }   
    for_each(threads.begin(),threads.end(),mem_fn(&thread::join));
    return 0;
}
