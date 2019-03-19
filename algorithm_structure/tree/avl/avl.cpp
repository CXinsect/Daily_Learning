#include "avl.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
int BST::getHeight(Node *node) {
    if(node == NULL) {
        return 0;
    }
    else {
        return node->height;
    }
}
int BST::getBalanceFactor(Node *node) {
    if(node == NULL) {
        return 0;
    }
    else {
        return getHeight(node->left) - getHeight(node->right);
    }
}
int BST::Max(int a,int b) {
    if( a > b ) {
        return a;
    }
    else if (a < b){
        return b;
    }
}

Node *BST::leftRotate(Node *node) {
    if(node == NULL) {
        return NULL;
    }
    Node *x = node->right;
    Node * t = x->left;
    //左旋转
    x->left = node;
    node->right = t;
    //更新节点高度
    node->height = 1 + Max(getHeight(node->left),getHeight(node->right));
    x->height = 1 + Max(getHeight(x->left),getHeight(x->right));
    return x;
}
Node *BST::rightRotate(Node *node) {
    if(node == NULL) {
        return NULL;
    }
    Node *x = node->left;
    Node *t = x->right;
    //右旋转
    x->right = node;
    node->left = t;
    //更新节点高度
    node->height = 1 + Max(getHeight(node->left),getHeight(node->right));
    x->height = 1 + Max(getHeight(x->left),getHeight(x->right));
    return x;
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
        return NULL;
    }
    else {
        if(node->data > el)
            node->left = add(node->left,el);
        else if(node->data < el)
            node->right = add(node->right,el);
    //更新高度
        node->height = Max(getHeight(node->left),getHeight(node->right)) + 1;
    //计算平衡因子
        int balance = getBalanceFactor(node);
    //平衡维护
    //LL
        if(balance > 1 && getBalanceFactor(node->left) >= 0) {
            return rightRotate(node);
        }
    //RR
        if(balance < -1 && getBalanceFactor(node->right) <= 0) {
            return leftRotate(node); 
        }
    //LR
        if(balance > 1 && getBalanceFactor(node->left) <= 0) {
            node -> left = leftRotate(node->left);
            return rightRotate(node);
        }
    //RL
        if(balance < -1 && getBalanceFactor(node->right) >= 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
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
        return NULL;
    }
    Node *retNode = NULL;
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
            retNode = cur;
        }
        else if(node->right == NULL) {
            Node *cur = node->left;
            delete node->left;
            node->left = NULL;
            size--;
            retNode = cur;
        }
        else {
            Node * suc = getmin(node->right);
            if(suc == NULL) {
                cout<<"the suc is wrong"<<endl;
                exit(0);
            }
            suc->right = deleteAny(node->right,suc->data);
            suc->left = node->left;
            node->left = node->right = NULL;
            delete node;
            retNode = suc;
        }
    }
    if(retNode == NULL) {
        return NULL;
    }
    //平衡维护
    int balance = getBalanceFactor(retNode);
    //LL
    if(balance > 1 && getBalanceFactor(retNode->left) >= 0) {
        return rightRotate(retNode);
    }
    //RR
    else if(balance < -1 && getBalanceFactor(retNode->right) <= 0) {
        return leftRotate(retNode);
    }
    //LR
    if(balance > 1 && getBalanceFactor(retNode->left) <= 0) {
        retNode->left = leftRotate(retNode->left);
        return rightRotate(retNode);
    }
    //RL
    if(balance < -1 && getBalanceFactor(retNode->right) >= 0) {
        retNode->right = rightRotate(retNode->right);
        return leftRotate(retNode);
    }
    return retNode;
}
bool BST::Is_avl_tree(Node * node) {
    if(node == NULL) {
        return true;
    }
    int balance = getBalanceFactor(node);
    if(balance > 1 || balance < -1) 
        return false;
    return Is_avl_tree(node->left) && Is_avl_tree(node->right);
}
void BST::Delete(Node *node) {
    if(node == NULL) {
        return;
    }
    else {
        if(node->left != NULL) {
            Delete(node->left);
        }
        if(node->right != NULL) {
            Delete(node->right);
        }
        delete node;
        node = NULL;
    }
}
