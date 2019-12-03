#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        map<int,int> a;
        int sum = 0;
        for(int i = 0; i< nums.size();i++) {
            sum += nums[i];
            a[sum]++;
        }
        sort(nums.begin(),nums.end());
        int min = 999;
        int vmin = 999;
        int ret = 0;
        for(int i = 0; i < nums.size();i++){
            int p1 = i+1;
            int p2 = nums.size()-1;
            int t = target - nums[i];
            
            while(p1 < p2) {
                if(t == nums[p1] + nums[p2]) {
                    return target;
                }
                else if(t > nums[p1] + nums[p2]) {
                    if( target- nums[p1] - nums[p2] - nums[i] < min ) {
                        min = target - nums[p1] - nums[p2] - nums[i]; 
                    }    
                    p1++;
                } 
                else {
                    if(nums[p1] + nums[p2] + nums[i] - target < min ) {
                        min = nums[p1] + nums[p2] + nums[i] - target;
                    }    
                    p2--;
                }
                   
            }
        }
        cout << min << endl;
        int t1,t2;
        if(a.find(target + min) != a.end() && a.find(target-min) != a.end())
           ret = ::min(target-min,target+min);
        else if(a.find(target+min) != a.end())  
            ret = target+min; 
        else if(a.find(target-min) != a.end())
            ret = target-min;
        return ret;
    }
};

int main (void) {
    vector<int> a = {3,-2,-5,3,-4};
    Solution s;
    int t = s.threeSumClosest(a,-1);
    cout << t << endl;
    return 0;
}