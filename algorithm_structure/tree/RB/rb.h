#pragma once
#include <iostream>
#include <stack>
#include <queue>
class Node
{
    static bool const RED = true;
    static bool const BLACK = false;
public:
    Node () {
        data = 0;
        left = NULL;
        right = NULL;
        color = RED;
    };
    Node(int e) {
        data = e;
        left = NULL;
        right = NULL;
        color = RED;
    }
public:
    int data;
    bool color;
    Node * left,*right;
};

class BST {
    static bool const RED = true;
    static bool const BLACK = false;
public:
    BST() {
        root = NULL;
        size = 0;
    }
//添加元素
private: Node * add(Node * node,int el);
//删除最小的元素
private: Node * deleteMin(Node *node);
//删除最大的元素
private: Node * deleteMax(Node *node);
//删除任意元素
private: Node *  deleteAny(Node *node,int el);
//左旋转
private: Node * leftRotate(Node *node);
//右旋转
private: Node * rightRotate(Node *node);
//判断是否为红节点
private: bool IsRed(Node *node);
//颜色翻转
private: void flipcolor(Node *node);
//释放资源
private: void deleteElem(Node *node);
public: void add(int el) {
              root  = add(root,el);
              root->color = BLACK;
          }
         void deleteElem() {
             deleteElem(root);
         }
         void preorder() {
             preorder(root);
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
public:
    int  delMax();
    bool contains(Node * node,int el);
    bool isEmpty();
    void traverse(Node node);
    void preorder(Node *node);
    Node* getmin(Node *node);
public:
    int size;
    Node * root;
};

