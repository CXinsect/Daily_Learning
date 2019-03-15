#pragma once
#include <iostream>
using namespace std;
typedef struct _lazy {
    int lazy;
    int value;
}lazy;
class Tree
{
public:
    Tree() {
        len = 0;
        larray = NULL;
        data = NULL;
    }
    Tree(int *a,int _len) {
        len = _len;
        data = new int [len];
        for(int i = 0;i<len;i++) {
            data[i] = a[i];
        }
        larray  = new lazy [4 * len];
    }
    ~Tree() {
        if(data != NULL) {
            delete [] data;
            data = NULL;
            len= 0;
        }
        if(larray != NULL) {
            delete [] larray;
            larray = NULL;
            len = 0;
        }
    }
public:
    int *data;
    int len;
    lazy *larray;
};
class seg_tree {
public:
    seg_tree() {
        ptr = NULL;
        len = 0;
    }
    seg_tree(int * a,int _len) {
        ptr = new Tree (a,_len);
        len = _len;
    }
    ~seg_tree() {
        if(ptr != NULL) {
            delete ptr;
            ptr = NULL;
            len = 0;
        }
    }
public:
    void create_seg_tree() {
        create_seg_tree(0,0,ptr->len - 1);
    }
    int query_seg_tree(int l,int r) {
        return query_seg_tree(0,0,ptr->len-1,l,r);
    }

    void set(int index,int el) {
        set(0,0,ptr->len - 1,index,el);
    }

    int sum(int a,int b);
    int set_section(int l,int r,int el) {
        return set_section(0,0,len - 1,l,r,el);
    }
private:
    //获取左孩子
    int leftNode (int index);
    //获取右孩子
    int rightNode (int index);
    //创建线段树
    void create_seg_tree(int index,int l,int r);
    //区间查找
    int query_seg_tree(int index,int l,int r,int ql,int qr);
    //定点更新
    void set(int index,int l,int r,int s_index,int el);
    //设置懒人标记
    void push_down(int index);
    //区间更新
    int set_section(int index,int l,int r,int ql,int qr,int el);
private:
    Tree * ptr;
    int len;
    int lazy;
};
