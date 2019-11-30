#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
using namespace std;

class Solution {
public:
    bool isAnagram(string s, string t) {
        map<int,int> res;
        if(s.size() != t.size())
            return false;
        for(int i = 0;i < s.size();i++) {
            res[s[i]]++;
        }
        map<int,int> ret;
        for(int i = 0;i < t.size();i++) {
            ret[t[i]]++;
        }
        for(int i = 0; i < t.size();i++) {
            if(res.find(t[i]) == res.end())
                return false;
            if(ret[t[i]] != res[t[i]]) return false;
            
        }
        return true;
    }
};

int main (void) {
    string s = "rat",t = "car";
    Solution tmp;
    if(tmp.isAnagram(s,t)) std::cout << "Hello" << std::endl;
    return 0;
}