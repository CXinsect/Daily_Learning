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
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* t = new ListNode(-1);
        ListNode* slow = t;
        ListNode* fast = head;
        t->next = head;
        while(fast != NULL && fast->next != NULL) {
            if(fast->val != fast->next->val) {
                if(slow->next != fast) slow->next = fast->next;
                else slow = fast;
            }
            fast = fast->next;
        }
        if(slow->next != fast) slow->next = fast->next;
        return t->next;
    }
    ListNode* create(void) {
        ListNode* head = new ListNode(0);
        ListNode* cur = head;
        // for(int i = 0;i < 5;i++) {
            // ListNode* newNode = new ListNode(i);
            cur->next = new ListNode(1);
            cur = cur->next;
            cur->next = new ListNode(1);
            cur = cur->next;
            // cur->next = new ListNode(1);
            // cur = cur->next;
            // cur->next = new ListNode(2);
            // cur = cur->next;
            // cur->next = new ListNode(3);
            // cur = cur->next;

        // }
        return head;
    }
    void print(ListNode* head){
        ListNode* cur = head->next;
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
    head = s.deleteDuplicates(head);
    s.print(head);
    return 0;
}
