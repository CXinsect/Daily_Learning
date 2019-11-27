#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int num;
        int i;
        int j = numbers.size()-1;
        vector<int> a;
        int flag = 0;
        for(int k = 0; k < numbers.size();k++) {
            i = k+1;
            while(i <= j) {
                
                int mid = i + (j-i)/2;
                
                if(numbers[mid] == target - numbers[k]) {
                    
                    flag = 1;
                    
                    a.push_back(++k);

                    a.push_back(++mid);

                    break;
                    
                }
                
                else if(numbers[mid] > target-numbers[k])
                    j = mid-1;
                
                else
                    i = mid+1;
            }
            if(flag == 1)
                break;
        }
        return a;
    }
};
int main (void) {
    vector<int> a = {2,3,4};
    Solution s;
    vector<int> b = s.twoSum(a,6);
    for(auto i : b) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}