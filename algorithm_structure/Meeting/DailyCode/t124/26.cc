#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int i = 0;
        for(int j = 1;j < nums.size();j++) {
            if(nums[i] != nums[j]) nums[++i] = nums[j];
        }
        return i+1;
    }
};

int main (void) {
    vector<int> nums = {0,0,1,1,1,2,2,3,3,4};
    Solution s;
    int t = s.removeDuplicates(nums);
    for(auto i : nums)
        cout << i << " ";
    cout << endl;
    cout << t << endl;
    return 0;
}