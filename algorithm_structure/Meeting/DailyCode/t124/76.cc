#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    string minWindow(string s, string t) {
        vector<int> freq (64,0);
        for(char c : t)  
            freq[c - 'A']++;
        int l = 0,r = -1;
        vector<int> freq2 (64,0);
        while(r + 1 < s.size()) {
            if(r - l + 1 < t.size())
                freq2[s[++r] - 'A']++;
            if(r - l + 1 == t.size()) {
                if(same(freq2,freq))
                    return string(freq2.begin(),freq2.end());
                else freq2[s[++r] - 'A']++;  
            }
            if(r - l + 1 > t.size()) {
                if(same(freq2,freq))
                    freq2[s[l++] - 'A']--;
                else freq2[s[++r] - 'A']++;
            }
        }
        return "";

    }
    bool same(vector<int>a,vector<int>b) {
        int i;
        for(i = 0;i < a.size();i++) {
            if(b[i] != 0) {
                if(a[i] == 0)
                    break;
            }
        }
        return i == a.size();
    }
};

int main (void) {
    string S = "ADOBECODEBANC";
    string T = "ABC";
    Solution s;
    string t = s.minWindow(S,T);
    cout << t << endl;
    return 0;
}