#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
      vector<int> freq (26,0);
      for(int i = 0;i < p.size();i++)
        freq[p[i] - 'a']++;

      int l = 0, r = -1;
      vector<int> freqs(26,0);
      vector<int> ret;
      while(r + 1 < s.size()) {
          
          freqs[s[++r] - 'a']++;

          if(r - l + 1 > p.size()) {
              freqs[s[l++] - 'a']--;
          }
          int i;
          if(r - l + 1 == p.size()) {
              for(i = 0;i < 26;i++) {
                  if(freq[i] != freqs[i])
                        break;
              }
              if(i == 26)
                ret.push_back(l);
          }
      }
      return ret;
    }
};

int main (void) {
    string s = "abab";
    string p = "ab";
    Solution tmp;
    vector<int> a = tmp.findAnagrams(s,p);
    for(auto i: a)
        cout << i << " ";
    cout << endl; 
    return 0;
}