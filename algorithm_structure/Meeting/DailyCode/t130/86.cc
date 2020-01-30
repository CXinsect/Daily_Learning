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
    //  ListNode* partition(ListNode* head, int x) {
    //     ListNode* phead = NULL;
    //     ListNode* cur = head;
    //     ListNode* node = head;
    //     ListNode* pNode = NULL;
    //     while(node != NULL && node->val != x ) {
    //         node = node->next;
    //     }
    //     pNode = node;
    //     phead = node;
    //     while(cur != NULL) {
    //         if(cur->val >= x) {
    //             pNode->next = cur;
    //             pNode = pNode->next;
    //         } else {
    //             cur->next = phead;
    //             // t->next = phead;
    //             phead = cur;
    //         }
    //         cur = cur->next;
    //     }
    //     return phead;
    // }
    ListNode* partition(ListNode* head, int x) {
        ListNode* ahead = new ListNode(0);
        ListNode* bhead = new ListNode(0);
        ListNode* acur = ahead;
        ListNode* bcur = bhead;
        while(head != NULL) {
            if(head->val >= x) {
                bcur->next = head;
                bcur = bcur->next;
            } else {
                acur->next = head;
                acur = acur->next;
            }
            head = head->next;
        } 
        bcur->next = NULL;
        acur->next = bhead->next;
        return ahead->next;     
    }
    ListNode* create(void) {
        ListNode* head = new ListNode(0);
        ListNode* cur = head;
        // for(int i = 0;i < 5;i++) {
            // ListNode* newNode = new ListNode(i);
            cur->next = new ListNode(5);
            cur = cur->next;
            cur->next = new ListNode(4);
            cur = cur->next;
            cur->next = new ListNode(2);
            cur = cur->next;
             cur->next = new ListNode(1);
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
    head = s.partition(head,2);
    s.print(head);
    return 0;
}
