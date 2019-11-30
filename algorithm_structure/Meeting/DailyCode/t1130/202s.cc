#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
using namespace std;
class Solution {
public:
    bool isHappy(int n) {
        if(n <= 0)
            return false;
        int r,t;
        vector<int> a;
        int flag = n;
        while(1) {
            a.clear();
            
            while(n > 0) {
                r = (n % 10);
                a.push_back(r);
                n = n/10;
            }
            for(int i = 0;i < a.size();i++)
                n += a[i]*a[i];
            if(n == 1) 
                return true;
            if(n == 4) return false;       
        }
    }
};
int main (void) {
    Solution s;
    if(s.isHappy(2)) cout << "hello" << endl;
    return 0;
}