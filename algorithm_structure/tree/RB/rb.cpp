#include <iostream>
#include "rb.h"
#include <unistd.h>
#include <stdlib.h>
using namespace std;

Node * BST::leftRotate(Node * node) {
    Node * x = node->right;
    //左旋
    node->right = x->left;
    x->left = node;
    x->color = node->color;
    node->color = BLACK;
    return x; 
}
Node *BST ::rightRotate(Node *node) {
    Node * x = node->left;
    //右旋
    node->left = x->right;
    x->right = node;
    x->color = node->color;
    node->color = BLACK;
    return x;
}
bool BST::IsRed(Node *node) {
    if(node == NULL) {
        return false;
    }
    return true;
}
void BST::flipcolor(Node *node) {
    node->color = RED;
    node->left->color = BLACK;
    node->right->color = BLACK;
}
Node * BST::add(Node * node,int el)
{
    if(node == NULL)
    {
        size++;
        return new Node(el);
    }
    else if(contains(node,el)) {
        cout<<"the elememt is already exist"<<endl;
        return node;
    }
    else
    {
        if(node->data > el)
             node->left = add(node->left,el);
        else if(node->data < el)
            node->right = add(node->right,el);
    }
    if(IsRed(node->right) && !IsRed(node->left)) {
        node = leftRotate(node);
    }
    if(IsRed(node->left) && IsRed(node->left->right)) {
        node->left = leftRotate(node->left);
        node = rightRotate(node);
    }
    if(IsRed(node->left) && IsRed(node->left->left)) {
        node = rightRotate(node);
    }
    if(IsRed(node->left)  && IsRed(node->right)) {
        flipcolor(node);
    }
    return node;
}
void BST::traverse(Node node)
{
    cout<<node.data<<endl;
}
void BST::preorder(Node *node)
{
    if(node == NULL)
    {
        cout<<"The tree is Empty"<<endl;
        return;
    }
    else
    {
        traverse(*node);
        if(node->left != NULL)
            preorder(node->left);
        if(node->right != NULL)
            preorder(node->right);

    }
}
bool BST:: isEmpty() {
    return size == 0;
}
bool BST::contains(Node *node,int el) {
    if(node == NULL) {
        return false;
    }
    else {
        if(node->data == el)
            return true;
        if(node->data > el) {
            return contains(node->left,el);
        }
        else {
            return contains(node->right,el);
        }
    }
}
void BST::deleteElem(Node * node) {
    if(node == NULL) {
        cout<<"the tree is empty"<<endl;
        return;
    }
    else {
        if(node->left != NULL) {
            deleteElem(node->left);
        }
        if(node->right != NULL) {
            deleteElem(node->right);
        }
        delete node;
        node = NULL;
    }
}
Node * BST::deleteMin(Node *node) {
    if(node == NULL) {
        cout<<"the tree is empty"<<endl;
        return node;
    }
    if (node->left == NULL) {
        Node *right = node->right;
        delete node->right;
        node->right = NULL;
        size--;
        return right;
    }
    node->left = deleteMin(node->left);
    return node;
}
Node *BST::deleteMax(Node *node) {
    if(node == NULL) {
        cout<<"the tree is empty"<<endl;
        return node;
    }
    if (node->right == NULL) {
        Node *leftNode = node->left;
        node->left = NULL;
        delete node->right;
        size--;
        return leftNode;
    }
    node->right = deleteMax(node->right);
    return node;
}
Node * BST::getmin(Node *node) {
    Node * ret = NULL;
    if(node == NULL) {
        cout<<"the tree is empty"<<endl;
        return NULL;
    }
    else {
        if(node->left == NULL) {
             ret = node;
        }
        else {
            node->left = getmin(node->left);
        }
    }
    cout<<"ret: "<<node->data<<endl;
    return ret;
}
Node * BST::deleteAny(Node *node,int el) {
    if(node == NULL) {
        cout<<"the tree is empty"<<endl;
        return node;
    }
    if(node->data > el) {
        node->left = deleteAny(node->left,el);
    }
    else if(node->data < el) {
        node->right = deleteAny(node->right,el);
    }
    else {
        if(node->left == NULL) {
            Node *cur = node->right;
            delete node->right;
            node->right = NULL;
            size--;
            return cur;
        }
        else if(node->right == NULL) {
            Node *cur = node->left;
            delete node->left;
            node->left = NULL;
            size--;
            return cur;
        }
        else {
            Node * suc = getmin(node->right);
            if(suc == NULL) {
                cout<<"the suc is wrong"<<endl;
                exit(0);
            }
            suc->right = deleteMin(node->right);
            suc->left = node->left;
            node->left = node->right = NULL;
            delete node;
            return suc;
        }
        
    }
    return node;
}
