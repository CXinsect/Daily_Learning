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
    ListNode* reverseList(ListNode* head) {
        ListNode* pre = NULL;
        if(head == NULL)
            return NULL;
        ListNode* curNode = head;
        while(curNode != NULL) {
            ListNode* next = curNode->next;
            curNode->next = pre;
            pre = curNode;
            curNode = next;
        }
        return pre;
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
    head = s.reverseList(head);
    s.print(head);
    return 0;
}
