#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <unordered_map>
using namespace std;

class Solution {
public:
    bool isIsomorphic(string s, string t) {
        if(s.size() != t.size())
            return false;
        map<int,int> sm;
        for(int i = 0; i < s.size(); i++) 
            sm[s[i]] = t[i];
        map <int,int> tm;
        for(int j = 0; j < t.size();j++) 
            tm[t[j]] = s[j];
        for(int i = 0 ; i < t.size();i++) {
            if(sm[s[i]] != t[i] || tm[t[i]] != s[i]) return false;
        }  
        return true;      
    }
};

int main (void) {
    string s = "foo",t = "bar";
    Solution tmp;
    if(tmp.isIsomorphic(s,t)) cout << "hello" <<endl;
    return 0;
}
