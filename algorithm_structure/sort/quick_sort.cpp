#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
template <typename T> 
class QuickSort {
    public:
        int quicksort1(vector <T> & array_,int l,int r) {
            //选定基值
            T v = array_[l];
            int j = l;
            for(int i = l+1;i < r;) {
                if(array_[i] < v) {
                    swap(array_[++j],array_[i++]);
                }
                else {
                    i++;
                }
            }
            //最后阶段：将数据序列分为[<v]---[v]---[>v]
            swap(array_[l],array_[j]);
            //将第一个排好顺序的元素下标返回
            return j;
        }
        int quicksort2(vector <T> &array_,int l,int r) {
            T v = array_[l];
            int i = l+1;
            int j = r-1;
            while(1) {
                while(array_[j] > v && j >= l+1) j--;
                while(array_[i] < v && i <= j) i++;
                if(i > j)
                    break;
                swap(array_[i++],array_[j--]);
            }
            swap(array_[l],array_[j]);
            return j;
        }
        void quicksort3(vector<T> &array_,int l,int r) {
            if(l >= r)
                return;
            T v = array_[l];
            int lt = l+1;
            int gt = r;
            for(int i = lt+1;i < gt;) {
                if(array_[i] > v) {
                    swap(array_[i],array_[--gt]);
                }
                else if(array_[i] < v)
                    swap(array_[i++],array_[++lt]);
                else i++;
            }
            swap(array_[l],array_[lt]);
            quicksort3(array_,l,lt-1);
            quicksort3(array_,gt,r);
        }
        void Sort1(vector<T> &array_,int l,int r) {
            if(l >= r)
                return;
            int p = quicksort1(array_,l,r);
            Sort1(array_,l,p-1);
            Sort1(array_,p+1,r);
        }
         void Sort2(vector<T> &array_,int l,int r) {
            if(l >= r)
                return;
            int p = quicksort2(array_,l,r);
            Sort2(array_,l,p-1);
            Sort2(array_,p+1,r);
        }
        void Quick1(vector<T> &array_,int l,int r) {
            Sort1(array_,l,r);
        }
        void Quick2(vector<T> &array_,int l,int r) {
            Sort2(array_,l,r);
        }
    // private:
    //     vector <T> array;
};
int main (void) {
    vector <int> v = {1,-1,3,6,2};
    QuickSort <int> s;
    s.quicksort3(v,0,v.size());
    for(int i = 0;i < v.size();i++)
                cout << v[i] << endl;
    return 0;
}