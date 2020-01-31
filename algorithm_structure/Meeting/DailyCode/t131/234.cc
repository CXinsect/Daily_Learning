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
    bool isPalindrome(ListNode* head) {
        if(head == NULL) return true;
        ListNode* cur = head;
        int len = 0;
        while(cur != NULL) {
            cur = cur->next;
            len++;
        }
        if(len == 1) return true;
        if(len % 2 == 0) {
            ListNode* slow = head;
            ListNode* fast = head;
            while(fast != NULL && fast->next != NULL) {
                fast = fast->next->next;
                slow = slow->next;
            }
            ListNode* p = reverse(slow);
            if(!p) return true;
            slow->next = NULL;
            while(head != slow && p != NULL) {
                if(head->val != p->val) return false;
                head = head->next;
                p = p->next;
            }
            return true;
            
        } else return false;
    }
    ListNode* reverse(ListNode* head) {
        if(head == NULL) return NULL;
        if(head->next == NULL) return head;
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
            // cur->next = new ListNode(i);
            // cur = cur->next;
            // cur->next = new ListNode(1);
            // cur = cur->next;
            // cur->next = new ListNode(2);
            // cur = cur->next;
            // cur->next = new ListNode(2);
            // cur = cur->next;
            // cur->next = new ListNode(1);
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
    if(s.isPalindrome(head->next)) cout << "hello" << endl;
    // s.print(head);
    return 0;
}
