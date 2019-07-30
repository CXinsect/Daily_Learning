#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
class Solution {
    public:
        Solution (std::string &a,std::string & b_) : a_(a),b(b_) {}
        
        int Solve() {
            int count = 0;
            transform(a_);
            for(int i = 0;i < v.size();i++) {
                if(b.find(v[i]) != -1)
                    count++;
                else continue;
            }
            return count;
        }
        private:
            void transform(std::string & a_ ) {
                int ret;
                int len = 1;
                for(int i = 0;i < len;i++) {
                     if(a_.find('?') != -1) {
                         a_[i] = '0';
                         continue;
                     }
                     
                    else
                        continue;
                }
               
            }
     private:
        std::vector <std::string> v;
        std::string a_;
        std::string b;
};
int main (void) {
    std::string a,b;
    std::cin >> a >> b;
    Solution s(a,b);
    std::cout << s.Solve() << std::endl;
    return 0;
}