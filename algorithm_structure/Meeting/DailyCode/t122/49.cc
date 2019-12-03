#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <set>
using namespace std;

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector< vector<string> > a;
        int p1 = 0;
        int p2 = strs.size()-1;
        vector<string> v;
        while(p1 < strs.size()) {
            if(isAnagram(strs[p1],strs[p2]))
                swap(strs[++p1],strs[p2]);
            else {
                p2--;
            }
            if(p2 <= p1) {
                 p2 = strs.size()-1;
                 for(int i = 0;i < p1;i++)
                    v.push_back(strs[i]);
                a.push_back(v);
                v.clear();
            }
        }
        return a;
    }
    bool isAnagram(string const& str,string const& strs) {
        map<int,int> a;
        map<int,int> b;
        for(int i = 0;i < str.size();i++) {
            a[str[i]]++;
            b[strs[i]]++;
        }
        for(int i = 0; i<strs.size();i++) {
            if(a[str[i]] != b[str[i]])
                return false;
        }
        return true;
    }
};

int main (void) {
    vector<string> a = {"eat", "tea", "tan", "ate", "nat", "bat"};
    Solution s;
    vector<vector<string>> t = s.groupAnagrams(a);
    for(int i = 0; i < t.size();i++) {
        for(auto j : t[i])
            cout << j << " "; 
         cout << endl;
    }
    cout << endl;
    if(s.isAnagram("tan","ate"))
        cout << "hello" << endl;
    return 0;
}