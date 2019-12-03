#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
        map<int,int> a;
        int flag;
        if(k == 0 || k == 10000)
            return false;
        for(int i = 0;i < nums.size();i++) {
            flag = t;
            while(flag >= 0) {
                if(a.find(nums[i] - flag) != a.end()) {
                    if(abs(a[nums[i]-flag] - i) <= k)
                        return true;
                }
                if(a.find(nums[i] + flag) != a.end()) {
                    if(abs(a[nums[i]+flag] - i) <= k)
                        return true;
                }
                flag--;
            }
            a[nums[i]] = i;
        }
        return false;
    }
};

int main (void) {
    vector<int> a = {4,1,-1,6,5};
    Solution s;
    if(s.containsNearbyAlmostDuplicate(a,3,1)) cout << "hello" << endl;
    return 0;
}