#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int min = m >= n ? n : m;
        vector<int> tmp;
        int index1 = 0,index2 = 0;
        while(index1 < m && index2 < n) {
            if(nums1[index1] <= nums2[index2])
                tmp.push_back(nums1[index1++]);
            else
                tmp.push_back(nums2[index2++]);
        }
        while(index1 < m)
            tmp.push_back(nums1[index1++]);
        while(index2 < n)
            tmp.push_back(nums2[index2++]);
        nums1.clear();
        for(int j = 0;j < tmp.size();j++)
            nums1.push_back(tmp[j]);
    }
};

int main (void) {
    vector <int> a = {1,2,3,0,0,0};
    vector <int> b = {2,5,6};
    Solution s;
    s.merge(a,3,b,3);
    for(auto i : a) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}

