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
    void reorderList(ListNode* head) {
        ListNode* c = head;
        // ListNode* ph = head;
        ListNode* slow = head;
        ListNode* fast = head;
        int len = 0;
        while(head != NULL) {
            head = head->next;
            len++;
        }
        while(fast != NULL && fast->next != NULL) {
            fast = fast->next->next;
            slow = slow->next;
        }
        ListNode* p;
        if(len % 2 == 0) {
            p = reverse(slow);
            slow->next = NULL;
            while(p->next != NULL) {
                ListNode* t = c->next;
                c->next = p;
                ListNode* tmp = p->next;
                // if(p->next == NULL)
                p->next = t;
                c = t;
                p = tmp;
            }       
        }
        else {
            if(!slow->next)
                return;
            p = reverse(slow->next);
            slow->next = NULL;
            while(p != NULL) {
                
                ListNode* t = c->next;
                c->next = p;
                ListNode* tmp = p->next;
                p->next = t;
                p = tmp;
                c = t;
            }
        }
    }
    ListNode* reverse(ListNode* head){
        ListNode* pre = NULL;
        ListNode* cur = head;
        while(cur != NULL) {
            ListNode* next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }
    ListNode* create(void) {
        ListNode* head = new ListNode(0);
        ListNode* cur = head;
        // for(int i = 0;i < 5;i++) {
            // ListNode* newNode = new ListNode(i);
            cur->next = new ListNode(1);
            cur = cur->next;
            // cur->next = new ListNode(1);
            // cur = cur->next;
           
            cur->next = new ListNode(2);
            cur = cur->next;
            cur->next = new ListNode(3);
            cur = cur->next;
             cur->next = new ListNode(4);
            cur = cur->next;
            cur->next = new ListNode(5);
            cur = cur->next;
            cur->next = new ListNode(6);
            cur = cur->next;
            // cur->next = new ListNode(7);
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
    s.reorderList(head->next);
    s.print(head);
    return 0;
}
