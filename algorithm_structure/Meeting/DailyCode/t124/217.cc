#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        set<int> table;
        for(int i = 0;i < nums.size();i++) {
            if(table.find(nums[i]) != table.end())
                return true;
            table.insert(nums[i]);
        }
        return false;
    }
};

int main (void) {
    vector<int> a{1,1,1,3,3,4,3,2,4,2};
    Solution s;
    if(s.containsDuplicate(a)) cout << "hello" <<endl;
    return 0;
}