#include <iostream>
#include <unistd.h>

using namespace std;


 //Definition for singly-linked list.
 struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
private:
    ListNode* suc = NULL;
public:
    ListNode* reverseN (ListNode* head,int m) {
        if(m == 1) {
            suc = head->next;
            return head;
        }
        ListNode* last = reverseN(head->next,m-1);
        head->next->next = head;
        head->next = suc;
        return last;
    }
    ListNode* reverseBetween(ListNode* head, int m, int n) {
        if(m == 1) {
            return reverseN(head,n);
        }
        head->next = reverseBetween(head->next,m-1,n-1);
        return head;
    }
    ListNode* create(void) {
        ListNode* head = new ListNode(0);
        ListNode* cur = head;
        for(int i = 0;i < 5;i++) {
            // ListNode* newNode = new ListNode(i);
            cur->next = new ListNode(i);
            cur = cur->next;
        }
        return head;
    }
    void print(ListNode* head){
        ListNode* cur = head;
        while(cur != NULL) {
            cout << cur->val << "->";
            cur = cur->next;
        }
        cout << "NULL" << endl;
    }
};

int main (void) {
    Solution s;
    ListNode* head = s.create();
    s.print(head);
    head = s.reverseBetween(head->next,2,4);
    s.print(head);
    return 0;
}
