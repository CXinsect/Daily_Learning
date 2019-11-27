#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        return qsort(nums,0,nums.size()-1,k);                                                                                                                                         
    }
    int qsort(vector<int>& nums,int _l,int _r,int k) {
        int a = sort(nums,_l,_r);
        if(a == k)
            return nums[k];
        qsort(nums,0,a-1,k);
        qsort(nums,a+1,nums.size()-1,k);
    }
    int sort(vector<int>& nums,int _l,int _r) {
        int a = nums[0];
        int i = _l;
        int lt = i+1;
        int j = _r-1;
        while(1) {
            while(nums[j] > a && j >= lt)
                j--;
            while(nums[i] < a && i <= j)
                i++;
            if(i > j)
                break;
            swap(nums[j],nums[i]);
        }
        return i;
    }
};

int main (void) {
    vector <int> a = {5,4,3,2,1};
    Solution s;
    std::cout << s.findKthLargest(a,2) << std::endl;

    return 0;
}