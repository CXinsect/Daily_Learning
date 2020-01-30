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
    bool hasCycle(ListNode *head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while(fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
            if(slow == fast)
                return true;
        }
        return false;
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
            cur->next = new ListNode(2);
            cur = cur->next;

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
