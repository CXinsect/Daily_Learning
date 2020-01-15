#ifndef __LRU_H_
#define __LRU_H_
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;
template <typename T>
struct LinkedNode {
    T key;
    T value;
    LinkedNode<T>* pre;
    LinkedNode<T>* next;
};

template <typename T>
class LRUCache {
    public:
        LRUCache (int cap) : count(0),capicaty(cap),head(NULL),tail(NULL){
            head = new LinkedNode<T>;
            tail = new LinkedNode<T>;
            head->next = tail;
            tail->pre = head;
        }
        int get (T key) {
            auto ret = cache.find(key);
            if(ret == cache.end()) {
                return -1;
            } else {
                moveToHead(&ret->second);
                return ret->second.value;
            }
        }

        void set (T key,T value) {
            auto ret = cache.find(key);

            if(ret == cache.end()) {
                LinkedNode<T>* newNode(new LinkedNode<T>);
                newNode->key = key;
                newNode->value = value;
                cache.insert(make_pair(key,*newNode));
                addNode(newNode);
                count++;
                if(count > capicaty) {
                    LinkedNode<T> tail = popTail();
                    cache.erase(tail.key);
                    count--;
                }
            } else {
                ret->second.value = value;
                moveToHead(&ret->second);
            }
        }
        int getCount() { return count; }
        // ~LRUCache() {
        //     while(head != NULL) {
        //         LinkedNode<T>* tmp = head->next;
        //         delete head;
        //         head = tmp;
        //     }
        // }
    private:
        void addNode(LinkedNode<T>* node) {
            node->pre = head;
            node->next = head->next;
            head->next->pre = node;
            head->next = node;
        }

        void removeNode(LinkedNode<T>* node) {
            // LinkedNode<T>* pre = node->pre;
            // LinkedNode<T>* next = node->next;
            // pre->next = next;
            // next->pre = pre;
            node->pre->next = node->next;
            node->next->pre = node->pre;
        }
        void moveToHead(LinkedNode<T>* node) {
            if(node != NULL) {
                removeNode(node);
                addNode(node);
            }
        }

        LinkedNode<T> popTail() {
            LinkedNode<T> res = *tail->pre;
            removeNode(&res);
            return res;
        }
    private:
        unordered_map<T,LinkedNode<T> > cache;
        int count;
        int capicaty;
        LinkedNode<T>*head, *tail;

};

#endif