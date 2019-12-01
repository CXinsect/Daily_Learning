#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace std;
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        map<int,int> m;
        for(int i = 0; i < nums.size();i++) {
            int t = target - nums[i];
            if(m.find(t) != m.end()) {
                int a[2] = {m[t],i};
                return vector<int>(a,a+2);
            }
            m[nums[i]] = i;
        }
        throw invalid_argument("Invaild Argument");
    }
};

int main (void) {
    vector<int> a = {3,3};
    Solution s;
    vector<int> b = s.twoSum(a,6);
    for(auto i : b) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}