#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        vector<int> freq(26,0);
        for(int i = 0;i < s1.size();i++) 
            freq[s1[i] - 'a']++;
        
        int l = 0, r = -1;
        vector<int> freq2(26,0);
        while(r + 1 < s2.size()) {
            freq2[s2[++r] - 'a']++;
            if(r - l + 1 > s1.size()) {
                freq2[s2[l++] - 'a']--;
            }
            int i;
            if(r - l + 1 == s1.size()) {
                for(i = 0;i < 26;i++) {
                    if(freq[i] != freq2[i])
                        break;
                }
                if(i == 26)
                    return true;
            }
        }
        return false;
    }
};

int main (void) {
    string s1 = "ab";
    string s2 = "eidboaoo";
    Solution s;
    if(s.checkInclusion(s1,s2)) cout << "hello" << endl;
    return 0;
}