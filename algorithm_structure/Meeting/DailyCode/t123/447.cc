#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
using namespace std;

class Solution {
public:
    int numberOfBoomerangs(vector<vector<int>>& points) {
        map<int,int>a;
        for(int i = 0;i < points.size();i++) {
            for(int j = 0;j < points.size();j++) {
                if(j != i)
                    a[s(points[i],points[j])]++;
            }
        }
        int ret = 0;
        for(map<int,int>::iterator iter = a.begin();
                iter != a.end();iter++) 
        {
            if(iter->second >=2 )
            ret += iter->second * (iter->second-1);
        }
        return ret;
    }
    int s(vector<int> le,vector<int> lr) {
        return (le[0] - lr[0]) * (le[0] - lr[0]) + (le[1]-lr[1]) * (le[1]-lr[1]);
    }
};

int main (void) {
    vector<vector<int>> a = {{0,0},{1,0},{2,0}};
    Solution s;
    int t = s.numberOfBoomerangs(a);
    cout << t << endl;
    return 0;
}