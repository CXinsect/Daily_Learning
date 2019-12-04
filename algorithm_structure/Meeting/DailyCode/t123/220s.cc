#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <vector>

using namespace std;

class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
        set<long long> table;
        
        for(int i = 0;i < nums.size();i++) {
             
            if(table.lower_bound((long long)nums[i]-t) != table.end() && *table.lower_bound((long long)nums[i]-t) <= (long long)nums[i]+t)
                return true;

            table.insert(nums[i]);

            if(table.size() == k + 1)
                table.erase(nums[i-k]);
        }
        return false;
    }
};

int main (void) {
    vector<int> a = {0,2147483647};
    Solution s;
    if(s.containsNearbyAlmostDuplicate(a,1,2147483647)) cout << "hello" << endl;
    return 0;
}