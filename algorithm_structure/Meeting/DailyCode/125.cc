
#include <iostream>
#include <vector>
#include <string>

//回文串不区分大小写
using namespace std;

class Solution {
public:
    bool isPalindrome(string s) {
        int i = 0;
        int j = s.size()-1;
        while(i < j) {
            if(!((s[i] >= 65 && s[i] <= 90) || (s[i] >=97 && s[i] <= 122) || (s[i] >=48 && s[i] <= 57)))
                i++;
            else if(!((s[j] >= 65 && s[j] <= 90) || (s[j] >=97 && s[j] <= 122) || (s[j] >=48 && s[j] <= 57)))
                j--;
            else if((s[i] != s[j]) && (s[i] != (s[j] - 32)) && (s[i]-32) != s[j]) {
               
                return false;
            } 
            else if(((s[j] == s[i]-32) && s[i]-32 < 65)) return false;
            else if((s[i] == s[j]-32) && (s[j] -32 ) < 65) return false;
            else {
                    i++;
                    j--;
            }
        }
        return true;
    }
};

int main (void) {
    string a = "0P";
    Solution s;
    if(s.isPalindrome(a))
        std::cout << "hello" << std::endl;
    else std::cout << "No" << std::endl;
    return 0;
}