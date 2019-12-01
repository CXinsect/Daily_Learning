#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int i = 0;
        vector<vector<int>> ret;
        sort(nums.begin(),nums.end());
        while(i < nums.size()) {
            if(nums[i] > 0)
                break;
            int index = i+1;
            int index1 = nums.size()-1;
            while(index < index1) {
                if(nums[index] + nums[index1] == -nums[i]) {
                    ret.push_back({nums[i],nums[index],nums[index1]});
                    index = next(nums,index);
                    index1 = pre(nums,index1);
                }
                else if(nums[index] + nums[index1] < -nums[i])
                    index = next(nums,index);
                else index1 = pre(nums,index1);
            }
            i = next(nums,i);
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
    vector<int> v{-1,0,1,2,-1,-4};
    Solution s;
    vector<vector<int>> a = s.threeSum(v);
    for(int i = 0; i < a.size();i++) {
        for(int k = 0;k < a[i].size();k++)
            cout << a[i][k] << " ";
        cout << endl;
    }
}