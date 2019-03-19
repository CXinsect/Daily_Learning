#pragma once
#include "array.cpp"
#include <iostream>
#include <stack>
#include <queue>
class Node
{
public:
    Node () {
        data = 0;
        left = NULL;
        right = NULL;
    };
    Node(int e) {
        data = e;
        height = 1;
        left = NULL;
        right = NULL;
    }
public:
    int data;
    int height;
    Node * left,*right;
};
class BST {
public:
    BST() {
        root = NULL;
        size = 0;
    }
    ~BST() {
        size = 0;
    }
//添加元素
private: Node * add(Node * node,int el);
//非递归遍历
private: void  NoCursion(Node *node);
//删除最小元素
private: Node * deleteMin(Node *node);
//删除最大元素
private: Node * deleteMax(Node *node);
//删除任意元素
private: Node *  deleteAny(Node *node,int el);

//更新节点高度
private:int getHeight(Node * node);
//计算平衡因子
private:int getBalanceFactor(Node * node);
//左旋转
private:Node * leftRotate(Node *node);
//右旋转
private:Node * rightRotate(Node *node);
//获取两数中较大值
private:int Max(int a,int b);
//判断是不是平衡二叉树
private:bool Is_avl_tree(Node *node);
//释放资源
private:void Delete(Node * node);

public: void add(int el) {
              root  = add(root,el);
          }

         void NoCursion() {
             NoCursion(root);
         }
         
         void deleteMin() {
             root = deleteMin(root);
         }

         void deleteMax() {
            root =  deleteMax(root);
         }

         void deleteAny(int el) {
             root = deleteAny(root,el);
         }
         bool Is_avl_tree() {
             return Is_avl_tree(root);
         }
         void Delete() {
             Delete(root);
         }
public:
    int  delMax();
    //判断是否插入重复元素
    bool contains(Node * node,int el);
    //判断树是否为空
    bool isEmpty();
    //输出节点值
    void traverse(Node node);
    //先序遍历
    void preorder(Node *node);
    //中序遍历
    void midorder(Node *node);
    //后序遍历
    void lastorder(Node *node);
    //层序遍历
    void levelOrder(Node *node);
    //删除元素
    void deleteElem(Node *node);
    //获得最小元素
    Node* getmin(Node *node);
    int getSize() {
        return size;
    }
public:
    int size;
    Node * root;
};

