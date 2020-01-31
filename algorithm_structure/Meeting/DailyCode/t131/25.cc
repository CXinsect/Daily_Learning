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
    ListNode* reverseKGroup(ListNode* head, int k) {
        if(head == NULL) return NULL;
        ListNode* node1 = head;
        ListNode* node2 = head;
        for(int i = 0;i < k;i++) {
           if(node2 == NULL)
                return head;
            node2 = node2->next;
        }
        ListNode* newHead = reverseN(node1,node2);
        node1->next = reverseKGroup(node2,k);
        return newHead;
    }
    ListNode* reverseN(ListNode* a,ListNode* b) {
       ListNode* pre = NULL;
       ListNode* cur = a;
       while(cur != b) {
           ListNode* next = cur->next;
           cur->next = pre;
           pre = cur;
           cur = next;
       }
       return pre;
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
