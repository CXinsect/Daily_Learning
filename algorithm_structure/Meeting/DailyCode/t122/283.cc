#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int i = 0;
        int j = 0;
        while(j < nums.size()) {
            if(nums[j])
                swap(nums[i++],nums[j]);
            j++;
        }
    }
};

int main (void) {
    vector<int> a = {2,1};
    Solution s;
    s.moveZeroes(a);
    for(auto i : a)
        std::cout << i << " ";
    cout << endl;
    return 0;
}