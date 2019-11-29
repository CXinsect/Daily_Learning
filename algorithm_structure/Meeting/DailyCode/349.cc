#include <iostream>
#include <set>
#include <map>
#include <vector>
using namespace std;

class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        map<int,int> res;
        for(int i = 0;i < nums1.size();i++) {
            res[nums1[i]]++;
        }
        vector <int> ret;
        for(int i = 0; i < nums2.size();i++) {
            if(res.find(nums2[i]) != res.end()) {   
                 if(res[nums2[i]]--)
                    ret.push_back(nums2[i]);
            }
        }
        return ret;
    }
};

int main (void) {
    vector<int> a = {1,2,2,1};
    vector<int> b = {2,2};
    Solution s;
    vector<int> tmp = s.intersection(a,b);
    for(auto i: tmp)
        std::cout << i << " ";
    std::cout << std::endl;
    return 0;
}