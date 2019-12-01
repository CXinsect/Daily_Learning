#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        map<int,int> table;
        vector<vector<int>> ret;
        for(int i = 0; i < nums.size();i++) {
            table[nums[i]]++;
        }
        if(table[0] > 2)
            ret.push_back({0,0,0});
        sort(nums.begin(),nums.end());
        vector<int>::iterator it = unique(nums.begin(),nums.end());
        nums.erase(it,nums.end());
        
        
        for(int i = 0;i < nums.size();i++) {
            for(int j = i+1;j < nums.size();j++) {
                if(nums[i] * 2 + nums[j] == 0 && table[nums[i]] >=2) {
                    ret.push_back({nums[i],nums[i],nums[j]});
                }
                else if(nums[j]*2 + nums[i] == 0 && table[nums[j]] >= 2)
                    ret.push_back({nums[i],nums[j],nums[j]});
                else {
                    int tmp = 0 - nums[i] - nums[j];
                    if(tmp > nums[j] && table[tmp] > 0)
                        ret.push_back({nums[i],nums[j],tmp});
               }
            }
        }
        return ret;
    }
};

int main (void) {
    vector<int> v{0,0,0};
    Solution s;
    vector<vector<int>> a = s.threeSum(v);
    for(int i = 0; i < a.size();i++) {
        for(int k = 0;k < a[i].size();k++)
            cout << a[i][k] << " ";
        cout << endl;
    }
}