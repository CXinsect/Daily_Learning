#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        sort1(nums,0,nums.size());
        return nums[nums.size()-k];
    }
    void sort1(vector<int>& nums,int l,int r) {
        for(int i = 0;i < nums.size();i++) {
            int min = i;
            for(int j = i+1;j < nums.size();j++) {
                if(nums[j] < nums[min])
                    min = j;
            }
            swap(nums[i],nums[min]);
        }
    }
};

int main (void) {
    vector<int>a = {4,3,2,1};
    Solution s;
    // s.sort1(a,0,a.size());
    // for(auto i : a)
    //     std::cout << i << " ";
    std::cout <<"Max" << s.findKthLargest(a,2) <<  std::endl;
    return 0;
}