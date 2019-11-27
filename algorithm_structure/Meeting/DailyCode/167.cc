#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int i = 0,j = numbers.size()-1;
        int sum = 0;
        vector<int> t;
        while(i < j) {
            sum = numbers[i] + numbers[j];
            if(sum == target) {
                t.push_back(++i);
                t.push_back(++j);
                return t;
            }
            else if(sum > target) j--;
            else  i++;
        }
        return t;
    }
};

int main (void) {
    vector<int> a = {2,7,11,15};
    Solution s;
    vector<int> b = s.twoSum(a,9);
    for(auto i : b) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}