#include <iostream>
#include <algorithm>
#include <vector>
#include <assert.h>

using namespace std;

class Solution {
public:
    void sortColors(vector<int>& nums) {
        int one = -1;
        int two = nums.size();
        for(int i = 0;i < two;) {
            if(nums[i] == 1) i++;
            else if(nums[i] == 0) swap(nums[++one],nums[i++]);
            else {
                assert(nums[i] == 2);
                swap(nums[--two],nums[i]);
            }
        }
    }
};

int main(void) {
    vector <int> a  = {2,1,0};
    Solution s;
    s.sortColors(a);
    for(auto i : a) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}