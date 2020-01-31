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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if(head == NULL) return NULL;
        ListNode* t = new ListNode(-1);
        t->next = head;
        ListNode* slow = t;
        ListNode* fast = t;
        // n = n + 1;
        while(n > 0) {
            fast = fast->next;
            n--;
        }
        // if(fast == NULL) {
        //     ListNode* del = slow->next;
        //     slow->next = del->next;
        //     delete del;
        // }
        // slow = slow->next;
        // fast = fast->next;
        while(fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
        ListNode* del = slow->next;
        slow->next = del->next;
        delete del;
        return t->next;
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
            // cur->next = new ListNode(3);
            // cur = cur->next;
            // cur->next = new ListNode(4);
            // cur = cur->next;

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
    head = s.removeNthFromEnd(head->next,2);
    s.print(head);
    return 0;
}
