#include <iostream>
#include "bst.h"
#include <unistd.h>
#include <stdlib.h>
using namespace std;
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
        size++;
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
void BST::midorder(Node *node)
{
    if(node == NULL)
    {
        cout<<"The tree is Empty"<<endl;
        return;
    }
    else
    {
        cout<<"mid"<<endl;
        if(node->left != NULL)
            midorder(node->left);
        traverse(*node);
        if(node->right != NULL)
            midorder(node->right);
    }
}
void BST::lastorder(Node * node) {
    if(node == NULL) {
        cout<<"the tree is empty"<<endl;
        return;
    }
    else {
        cout<<"last traverse"<<endl;
        if(node->left != NULL) {
            lastorder(node->left);
        }
        if(node->right != NULL) {
            lastorder(node->right);
        }
        traverse(*node);
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
void BST::NoCursion(Node * node) {
    if(node == NULL) {
        cout<<"thr tree is empty"<<endl;
        return;
    }
    else {
        stack <Node *> s;
        s.push(node);
        while(!s.empty()) {
            Node * cur = s.top();
            cout<<cur->data<<endl;
            s.pop();
            if(cur->right != NULL) {
                s.push(cur->right);
            }
            if(cur->left != NULL) {
                s.push(cur->left);
            }
        }
    }
}
void BST::levelOrder(Node *node) {
    if(node == NULL) {
        cout<<"the tree is empty"<<endl;
        return;
    }
    else {
        queue <Node *> q;
        q.push(node);
        while(!q.empty()) {
            Node * cur = q.front();
            cout<<cur->data<<endl;
            q.pop();
            if(cur->left != NULL) {
                q.push(cur->left);
            }
            if(cur->right != NULL) {
                q.push(cur->right);
            }
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
