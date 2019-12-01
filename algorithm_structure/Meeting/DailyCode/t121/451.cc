#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Solution {
public:
    struct compare {
        bool operator() (const pair<int,int>& lhs, const pair<int,int>& rhs) {
            return lhs.second > rhs.second;
        }
    };
    string frequencySort(string s) {
        map<char,int> m;
        for(int i = 0; i< s.size();i++) {
            m[s[i]]++;
        }
        vector<pair<char,int>> ret(m.begin(),m.end());
        sort(ret.begin(),ret.end(),compare());
        vector<pair<char,int>>::iterator iter = ret.begin();
        string res;
        while(iter != ret.end()) {
            int n = iter->second;
            while(n != 0) {
                res += iter->first;
                n--;
            }     
            std::cout << iter->first << std::endl;     
            iter++;        
        }
         return res;
    }
    
};

int main (void) {
    string a = "tree";
    Solution s;
    string res = s.frequencySort(a);
    std::cout << res << std::endl;
    return 0;
}