#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <map>
using namespace std;

class Solution {
public:
    bool wordPattern(string pattern, string str) {
        map<string,int> s;
        istringstream split(str);
        string tmp;
        vector<string> t;
        int index = 0;
        while(split >> tmp) { s[tmp] = pattern[index++]; t.push_back(tmp); };
        map <int,int> p;
        for(int i = 0; i<pattern.size();i++) {
            p[pattern[i]] = s[t[i]];
        }
        if(p.size() != s.size()) return false;
        for(int i = 0 ; i < s.size();i++) {
            if( s[t[i]]!= pattern[i] || p[pattern[i]] != s[t[i]]) {
                 return false;
            }       
        }
        return true;
    }
};

int main (void) {
    string p = "abba";
    string str = "dog cat dog cat";
    Solution s;
    if(s.wordPattern(p,str)) cout << "hello" << std::endl;
    return 0;
}