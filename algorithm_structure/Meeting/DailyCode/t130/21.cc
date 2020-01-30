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
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* a = l1;
        ListNode* b = l2;
        ListNode* c = new ListNode(-1);
        ListNode* head = c;
        while(a != NULL && b != NULL) {
            if(a->val > b->val) {
                c->next = b;
                c = c->next;
                b = b->next;
            } else {
                c->next = a;
                c = c->next;
                a = a->next;
            }
        }
        if(a != NULL) {
            c->next = a;
        }
        if(b != NULL) {
            c->next = b;
        }
        return head->next;
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
