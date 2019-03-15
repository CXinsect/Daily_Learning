#include "Trie.h"
using namespace std;

int Trie::GetSize() {
    return size;
}
bool Trie::IsEmpty() {
    return size == 0;
}
//创建字典树
void Trie::add(Node * root,string word,int value) {
    Node *cur = root;
    char *buf = new char [word.length()];
    word.copy(buf,word.length(),0);
    int len = word.length();
    multimap<char,Node *> ::iterator it;
    for(int i = 0;i < len;i++) {
        //查找以buf[i]字符为键值的映射
        it = cur->next.find(buf[i]);
        //如果不存在，则新创建一个节点
        if(it == cur->next.end()) {
            cur->next.insert(pair<char,Node *>(buf[i],new Node ()));
        }
        //存在则继续向下遍历
        cur = cur->next.find(buf[i])->second;
    }
    //如果字符串不存在，则更新标记
    if(!cur->Word) {
        cur->Word = true;
        cur->nums = value;
        size++;
    }
    if(buf != NULL) {
        delete [] buf;
        buf = NULL;
    }
    return;
}
//字符串的匹配
bool Trie::contains(Node * root,string word) {
    Node * cur = root;
    char *buf = new char [word.length()];
    int len = word.length();
    word.copy(buf,len,0);
    multimap <char,Node*> ::iterator it;
    for(int i = 0;i < len;i++) {
        it = cur->next.find(buf[i]);
        if(it == cur->next.end()) {
            return false;
        }
        cur = cur->next.find(buf[i])->second;
    }
    if(buf != NULL) {
        delete [] buf;
        buf = NULL;
    }
    return cur->Word;
}

int Trie::sum(Node * root) {
    int val = root->nums;
    multimap<char,Node *>::iterator it = root->next.begin();
    //遍历以该节点为键值的所有字符串，统计合法字符串的数量
    while(it != root->next.end()) {
        char c = it->first;
        val += sum(root->next.find(c)->second);
        it++;
    }
    return val;
}
//统计前缀字符prefix出现的次数
int Trie::prefix_nums(Node * root,string prefix) {
    Node * cur = root;
    int len = prefix.length();
    char * buf = new char [len];
    prefix.copy(buf,len,0);
    multimap<char,Node*>::iterator it;
    for(int i = 0;i < len;i++) {
        it = cur->next.find(buf[i]);
        if(it == cur->next.end())
            cur->nums = 0;
        cur = cur->next.find(buf[i])->second;
    }
    return sum(cur);
}
