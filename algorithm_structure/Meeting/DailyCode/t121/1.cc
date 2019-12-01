#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
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

}