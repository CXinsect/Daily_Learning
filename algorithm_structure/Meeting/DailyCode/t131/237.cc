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
    void deleteNode(ListNode* node) {
        if(node == NULL) return;
        if(node->next == NULL) {
            delete node;
            node = NULL;
        }
        node->val = node->next->val;
        ListNode* del = node->next;
        node->next = del->next;
        delete del;    
    }
private:
    ListNode* suc = NULL;
public:
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
    head = s.reverseKGroup(head->next,2);
    s.print(head);
    return 0;
}
