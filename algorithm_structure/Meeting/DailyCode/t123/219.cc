#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
using namespace std;

class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        map<int,int> a;
        for(int i = 0;i < nums.size();i++) {
            if(a.find(nums[i]) != a.end()) {
                if(abs(a[nums[i]] - i) <=k )
                    return true;
            }
            a[nums[i]] = i;
        }
        return false;     
    }
};

int main (void) {
    vector<int> a = {1,2,3,1};
    Solution s;
    if(s.containsNearbyDuplicate(a,3)) cout << "hello" << endl;

}