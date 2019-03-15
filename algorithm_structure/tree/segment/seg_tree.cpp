#include "seg_tree.h"
using namespace std;

int seg_tree::leftNode(int index) {
    return index * 2 + 1;
}
int seg_tree::rightNode(int index) {
    return index * 2 + 2;
}
int seg_tree::sum(int a,int b) {
    return a + b;
}
void seg_tree::push_down(int index) {
    if(index < 0 || index >= len) {
        cout<<"The index is Wrong" <<endl;
        return;
    }
    if(ptr->larray[index].lazy != 0) {
        int leftindex = leftNode(index);
        int rightindex = rightNode(index);
        //lazy值向下传递
        ptr->larray[leftindex].lazy += ptr->larray[index].lazy;
        ptr->larray[rightindex].lazy += ptr->larray[index].lazy;
        //更新数组中元素的值
        ptr->larray[leftindex].value += ptr->larray[index].lazy;
        ptr->larray[rightindex].value += ptr->larray[index].lazy;
        //将标记置为零
        ptr->larray[index].lazy = 0;
    }
}
void seg_tree::create_seg_tree(int index,int l,int r) {
    ptr->larray[index].lazy = 0;
    if(l == r) {
        ptr->larray[index].value = ptr->data[l];
        return;
    }
    int mid = l + (r - l)/2;
    int leftindex = leftNode(index);
    int rightindex = rightNode(index);
    create_seg_tree(leftindex,l,mid);
    create_seg_tree(rightindex,mid+1,r);
    ptr->larray[index].value = sum(ptr->larray[leftindex].value,
                                   ptr->larray[rightindex].value);
}
int seg_tree::query_seg_tree(int index,int l,int r,int ql,int qr) {
    if(index < 0 || index >=len || l < 0 ||l >= len 
       || r < 0 || r >= len || r < l || ql < 0 || ql >= len || qr < 0 || qr >= len
       || qr < ql) {
        cout<<"The index is Wrong func [query]"<<endl;
        return -1;
    }
    if(l == ql && r == qr) {
        return ptr->larray[index].value;
    }
    push_down(index);
    int mid = l+ (r - l)/2;
    int leftindex = leftNode(index);
    int rightindex = rightNode(index);
    if(ql >= mid +1) {
        return query_seg_tree(rightindex,mid+1,r,ql,qr);
    }
    if(qr <= mid) {
        return query_seg_tree(leftindex,l,mid,ql,qr);
    }
    int lret = query_seg_tree(leftindex,l,mid,ql,mid);
    int rret = query_seg_tree(rightindex,mid + 1,r,mid + 1,qr);
    ptr->larray[index].value = sum(ptr->larray[lret].value,ptr->larray[rret].value);
}
void seg_tree::set(int index,int l,int r,int s_index,int el) {
    if(l == r) {
        ptr->larray[index].value = el;
        return;
    }
    push_down(index);
    int mid = l + (r - l) /2;
    int leftindex = leftNode(index);
    int rightindex = rightNode(index);
    if(s_index >= mid + 1) {
        set(rightindex,mid + 1,r,s_index,el);
    }
    else if(s_index <= mid) {
        set(leftindex,l,mid,s_index,el);
    }
    ptr->larray[index].value = sum(ptr->larray[leftindex].value,
                                   ptr->larray[rightindex].value);
}
int seg_tree::set_section(int index,int l,int r,int ql,int qr,int el) {
    if(index < 0 || index >= len || l < 0 || l >= len || r < 0 || r >= len ||
       ql < 0 || ql >= len || qr < 0 || qr >= len) {
        cout<<"The index is Wrong"<<endl;
        return -1;
    }
    if(l == ql && r == qr) {
        ptr->larray[index].lazy += el;
        ptr->larray[index].value += el;
        return ptr->larray[index].value;
    }
    //向下标记
    push_down(index);
    int mid = l + (r - l) /2;
    int leftindex = leftNode(index);
    int rightindex = rightNode(index);
    if(ql >= mid + 1) {
        return set_section(rightindex,mid + 1,r,ql,qr,el);
    }
    if(qr <= mid) {
        return set_section(leftindex,l,mid,ql,qr,el);
    }
    int lret = set_section(leftindex,l,mid,ql,mid,el);
    int rret = set_section(rightindex,mid + 1,r,mid + 1,qr,el);
    ptr->larray[index].value = sum(ptr->larray[lret].value,ptr->larray[rret].value);
}
