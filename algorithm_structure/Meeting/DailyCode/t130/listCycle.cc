#include <iostream>
#include <unistd.h>
#include <stack>
#include <vector>
#include <deque>

using namespace std;

struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
};
class Monotonic {
    public:
        void push(int n) {
            while(!data.empty() && data.back() < n) {
                data.front();
            }
            data.push_back(n);
        }
        int max() { return data.front(); }
        void pop (int n) {
            if(!data.empty() && data.front() == n)
                data.pop_front();
        }
    private:
        deque<int> data;

};
class Solution {
    public:
        bool hasCycle(ListNode* head) {
            ListNode* slow = head;
            ListNode* fast = head;
            while(fast != NULL && fast->next != NULL) {
                fast = fast->next->next;
                slow = slow->next;
                if(slow == fast)
                    return true;
            }
            return false;
        }
        vector<int> nextGreaterElement(vector<int>& nums) {
            vector<int> ret(nums.size());
            stack<int> s;
            for(int i = nums.size()-1;i >= 0;i--) {
                while(!s.empty() && s.top() <= nums[i])
                    s.pop();
                ret[i] = ret.empty() ? -1 : s.top();
                s.push(nums[i]);
            }
            return ret;
        }

        vector<int> slipwindow(vector<int>& nums,int k) {
            Monotonic win;
            vector<int> ret;
            for(int i = 0;i < nums.size();i++) {
                if(i < k - 1) {
                    win.push(nums[i]);
                } else {
                    win.push(nums[i]);
                    ret.push_back(win.max());
                    win.pop(nums[i-k+1]);
                }
            }
            return ret;
        }
    
};


int main (void) {
    return 0;
}