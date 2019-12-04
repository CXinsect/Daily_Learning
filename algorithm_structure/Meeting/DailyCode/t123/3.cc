#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        set<int> table;
        
        int l = 0,r = -1;
        int len = 0;
        while(r + 1 < s.size()) {
            if(table.find(s[r+1]) != table.end()) {
                table.erase(s[l++]);
            } else table.insert(s[++r]);
            int t = table.size();
            len = max(len,t);
        }        
        return len;  
    }
};

int main (void) {
    string s = "pwwkew";
    Solution tmp;
    int t = tmp.lengthOfLongestSubstring(s);
    cout << t << endl;
    return 0;
}