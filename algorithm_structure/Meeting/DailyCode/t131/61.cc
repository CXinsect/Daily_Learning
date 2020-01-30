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
    ListNode* rotateRight(ListNode* head, int k) {
      if(head == NULL)
        return NULL;
        ListNode* node = head;
        long int len = 0;
        while(head != NULL) {
            head = head->next;
            len++;
        }
        head = node;
        if(k > len)
            k = k % len;
        while(k > 0) {
            while(node != NULL && node->next != NULL && node->next->next != NULL )
                node = node->next;
            if(node->next != NULL) {
                ListNode* Tail = node->next;
                node->next = NULL;
                if(Tail != NULL)
                    Tail->next = head;
                head = Tail;
                k--;
                node = head;
            } else {
                return head;
            }
                
                
        }
        return head;
        
    }
    ListNode* create(void) {
        ListNode* head = new ListNode(0);
        ListNode* cur = head;
        // for(int i = 0;i < 5;i++) {
            // ListNode* newNode = new ListNode(i);
            // cur->next = new ListNode(i);
            // cur = cur->next;
            cur->next = new ListNode(0);
            cur = cur->next;
            cur->next = new ListNode(1);
            cur = cur->next;
            cur->next = new ListNode(2);
            cur = cur->next;
            // cur->next = new ListNode(3);
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
    head = s.rotateRight(head->next,4);
    s.print(head);
    return 0;
}
