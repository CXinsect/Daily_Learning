#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <iostream>
using namespace std;


class Solution {
public:
    int fourSumCount(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {
        map<int,int> a;
        for(int i = 0;i < A.size();i++) {
            for(int j = 0;j < B.size();j++) {
                a[0-A[i] - B[j]]++;
            }
        }
        int ret = 0;
        for(int i = 0; i < C.size();i++) {
            for(int j = 0;j < D.size();j++) {
                if(a.find(C[i]+D[j]) != a.end())
                    ret += a[C[i]+D[j]];
            }
        }
        return ret;
    }
};

int main (void) {
    vector<int>a = {-1,-1};
    vector<int>b = {-1,1};
    vector<int>c = {-1,1};
    vector<int>d = {1,-1};
    Solution s;
    int t = s.fourSumCount(a,b,c,d);
    cout << t << endl;
    return 0;
}