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
    ListNode* swapPairs(ListNode* head) {
        ListNode* t = new ListNode(-1);
        t->next = head;
        if(head == NULL)
            return NULL;
        ListNode* pre = t;
        ListNode* node1 = head;
        ListNode* node2 = node1->next;
        while(node1 != NULL && node2 != NULL) {
            ListNode* next = node2->next;
            node2->next = node1;
            node1->next = next;
            pre->next = node2;

            pre = node1;
            node1 = pre->next;
            if(node1 != NULL)
                node2 = node1->next;
        }
        return t->next;
            
    }
    ListNode* create(void) {
        ListNode* head = new ListNode(0);
        ListNode* cur = head;
        // for(int i = 0;i < 5;i++) {
            // ListNode* newNode = new ListNode(i);
            // cur->next = new ListNode(i);
            // cur = cur->next;
            cur->next = new ListNode(1);
            cur = cur->next;
            cur->next = new ListNode(2);
            cur = cur->next;
            cur->next = new ListNode(3);
            cur = cur->next;
            cur->next = new ListNode(4);
            cur = cur->next;

        // }
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
    head = s.swapPairs(head->next);
    s.print(head);
    return 0;
}
