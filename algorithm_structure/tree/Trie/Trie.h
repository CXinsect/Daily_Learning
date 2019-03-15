#pragma once

#include <iostream>
#include <map>
#include <memory>

using namespace std;

class Node {
 public:
     Node() {
         Word = false;
         nums = 0;
     }
     Node(bool IsWord,int num) {
         Word = IsWord;
         nums = num;
     }
     ~Node() {}
 public:
     bool Word;
     int nums;
     multimap <char,Node*> next;  
};
class Trie
{
public:
    Trie() {
        size = 0;
        root = new Node ();
    }
    ~Trie() {
        if(root != NULL) {
            delete root;
            root = NULL;
        }
    }
    //获取元素个数
    int   GetSize();
    //判断元素是否为空
    bool  IsEmpty();
    void add (string word,int num) {
        add (root,word, num);
    }
    bool contains(string word) {
        return contains(root,word);
    }
    int prefix_nums(string prefix) {
        return prefix_nums(root,prefix);
    }
    
private:
    //创建子字典树
    void  add (Node * root,string word,int num);
    //字符串的匹配（与前缀匹配类似）
    bool contains(Node * root,string word);
    //返回前缀字符串的个数
    int prefix_nums(Node * root,string prefix);
    //累计prefix出现的个数
    int sum(Node * node);
private:
    Node * root;
    int size;
};

