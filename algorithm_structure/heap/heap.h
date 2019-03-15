#pragma once
#include "array.cpp"
class heap
{
public:
    heap() {}
    heap(int capicaty) {
        array b(capicaty);
        a = b;
    }
    ~heap() {}
private:
    //获取父亲结点下标
    int parent(int index) {
        return (index - 1)/2;
    }
    //获取左孩子结点下标
    int leftChild(int index) {
        return index * 2 + 1;
    }
    //获取右孩子结点下标
    int rightChild(int index) {
        return index * 2 + 2;
    }
    //获取最大值
    int getMax() {
        int ret = a.get(0);
        return ret;
    }
    //向堆中添加元素
    void SiftUp(int index);
    //从堆中获取最大值
    void SiftDown(int index);
public:
    void add(int el);
    int exMax();
    void print();
    void Free();
private:
    array a;
};

