#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> ret;
        sort(nums.begin(),nums.end());
        for(int i = 0; i < nums.size();i = next(nums,i)) {
            for(int j = i+1; j < nums.size();j = next(nums,j)) {
                int t = target - nums[i] - nums[j];
                int index = j+1;
                int index1 = nums.size()-1;
                while(index < index1) {
                    if(t == nums[index1] + nums[index]) {
                        ret.push_back({nums[i],nums[j],nums[index],nums[index1]});
                        index = next(nums,index);
                        index1 = pre(nums,index1);
                    }
                    else if(nums[index] + nums[index1] < t)
                        index = next(nums,index);
                    else index1 = pre(nums,index1);
                }
            }
        }
        return ret;
    }
    int pre(vector<int>&nums, int cur) {
        for(int i = cur-1;i >=0; i--) {
            if(nums[cur] != nums[i])
                return i;
        }
        return nums.size();
    }
    int next (vector<int>&nums, int cur) {
        for(int i = cur+1;i < nums.size(); i++) {
            if(nums[cur] != nums[i])
                return i;
        }
        return nums.size();
    }
};

int main (void) {
     vector<int> v{1, 0, -1, 0, -2, 2};
    Solution s;
    vector<vector<int>> a = s.fourSum(v,0);
    for(int i = 0; i < a.size();i++) {
        for(int k = 0;k < a[i].size();k++)
            cout << a[i][k] << " ";
        cout << endl;
    }
    return 0;
}