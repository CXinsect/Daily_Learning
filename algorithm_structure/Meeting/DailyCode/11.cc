#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    int maxArea(vector<int>& height) {
        int i = 0;
        int j = height.size()-1;
        int area = -1;
        for(int k = 0;k < height.size();k++) {
            i = k+1;
            while(i < height.size()) {
                int m = min(height[i],height[k]);
                int tmp = (i-k)*m;
                if(tmp > area)
                    area = tmp;
                i++;
            }
        }
        return area;
        
    }
};

int main (void) {
    vector<int> a = {1,8,6,2,5,4,8,3,7};
    Solution s;
    int t =s.maxArea(a);
    std::cout << t << std::endl;
    return 0;
}