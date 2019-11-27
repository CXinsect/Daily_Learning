#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int l = 0,r = -1;
        int ret = 0;
        int fc[256] = {0};
        while(r+1 < s.size()) {
            if(fc[s[r+1]] == 0 && (r+1) < s.size()) {
                fc[s[++r]]++;
            }
            else {
                fc[s[l++]]--;
            }
            ret = max(ret,r-l+1);
        }
        if(ret == -1)
            return 0;
        return ret;
    }
};

int main (void) {
    string a = "abcabcbb";
    Solution s;
    string dst;
    int t = s.lengthOfLongestSubstring(a);
    std::cout << "length" << t << std::endl;
    return 0;
}