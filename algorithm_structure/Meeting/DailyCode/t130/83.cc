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
        if(head == NULL)
            return NULL;
        ListNode* cur = head;
        ListNode* next = cur->next;
        while(cur != NULL && next != NULL) {
            if(cur->val == next->val) {
                ListNode* delNode = next;
                cur->next = next->next;
                delete delNode;

            } else {
                cur = cur->next;
                // next = next->next;
            }
                next = cur->next;

        }
        return head;
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
