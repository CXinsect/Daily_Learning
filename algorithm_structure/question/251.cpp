#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        Quick(nums,0,nums.size()-1,k);
        int n = nums.size() - k;
        return nums[n];
    }
    int quicksort(vector <int> &nums,int l,int r) {
        int v = nums[l];
        int i = l+1,j = r;
        while(1) {
            while(nums[i] < v && i <= j) i++;
            while(nums[j] > v && j >= l+1) j--;
            if(i > j)
                break;
            swap(nums[i++],nums[j--]);
        }
        swap(nums[l],nums[j]);
        return j;
    }
    void Sort(vector <int> &nums,int l,int r,int k) {
        if(l >= r)
        return;
        int p = quicksort(nums,l,r);
        if(p+1 > k)
            Sort(nums,p+1,r,k);
        else if(p+1 < k)
            Sort(nums,l,p-1,k);
    }
    void Quick(vector <int> &nums,int l,int r,int k) {
        Sort(nums,l,r,k);
    }
};

int main (void) {
    vector <int> v = {3,2,3,1,2,4,5,5,6};
    Solution s;
    cout << s.findKthLargest(v,4) << endl;
    return 0;
}