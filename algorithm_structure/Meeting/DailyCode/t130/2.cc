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
private:
    ListNode* suc = NULL;
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int count = 1;
        int sum1= 0,sum2 = 0;
        ListNode* node1 = l1;
        ListNode* node2 = l2;
        while(node1 != NULL || node2 != NULL) {
            if(node1 != NULL)
            sum1 += node1->val*count;
            if(node2 != NULL)
            sum2 += node2->val*count;
            count *= 10;
            if(node1 != NULL)
            node1 = node1->next;
            if(node2 != NULL)
            node2 = node2->next;
        }
        int sum = sum1 + sum2;
        ListNode* head = NULL;
        int r = sum % 10;
        head = new ListNode(r);
        ListNode* cur = head;
        sum /= 10;
        while(sum > 0) {
            r = sum % 10;
            cur->next = new ListNode(r);
            sum /= 10;
            cur = cur->next;    
        }
        return head;
    }
    ListNode* create(void) {
        ListNode* head = new ListNode(0);
        ListNode* cur = head;
        // for(int i = 0;i < 5;i++) {
        //     // ListNode* newNode = new ListNode(i);
        //     cur->next = new ListNode(i);
        //     cur = cur->next;
        // }
        cur->next = new ListNode(1);
        cur = cur->next;
        cur->next = new ListNode(8);
        cur = cur->next;
       
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
    ListNode* head1 = new ListNode(0);
    head1->next = new ListNode(0);
    ListNode* head2 = s.addTwoNumbers(head->next,head1->next);
    s.print(head2);
    return 0;
}
