#pragma once
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
        left = NULL;
        right = NULL;
    }
public:
    int data;
    Node * left,*right;
};
class BST {
public:
    BST() {
        root = NULL;
        size = 0;
    }

private: Node * add(Node * node,int el);
private: void  NoCursion(Node *node);
private: Node * deleteMin(Node *node);
private: Node * deleteMax(Node *node);
private: Node *  deleteAny(Node *node,int el);

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
public:
    int  delMax();
    bool contains(Node * node,int el);
    bool isEmpty();
    void traverse(Node node);
    void preorder(Node *node);
    void midorder(Node *node);
    void lastorder(Node *node);
    void levelOrder(Node *node);
    void deleteElem(Node *node);
    Node* getmin(Node *node);
public:
    int size;
    Node * root;
};

