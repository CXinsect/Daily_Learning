#include <iostream>
#include <algorithm>
#include <vector>
class Solution {
    public:
        Solution(int n_) : n(n_) {}
        int solve() {
            std::vector <int> a(10);
            for(int i = 1;i <= n;i++) {
                a[i] = i;
            }
           
            int ret = a[1];
            for(int i = 2;i <= n;i++)  {
                 ret = divide(ret,a[i]);
            }
               
            return ret;
        }
        private:
        
        int divide (int x,int y) {
            int r,n1,n2;
            n1 = x;
            n2 = y;
            if(n1 < n2) 
                std::swap(n1,n2);
            while(n2 != 0) {
                r = n1;
                n1 = n2;
                n2 = r % n2;
            }
            int ret = (x*y)/n1;
            ret %= 987654321;
            return ret;
        }
    private:
        int n;
};
int main (void) {
    int n;
    std::cin >> n;
    Solution s(n);
    std::cout << s.solve() << std::endl;

}