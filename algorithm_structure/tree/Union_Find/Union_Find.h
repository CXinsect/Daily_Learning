#pragma once
#include <iostream>

using namespace std;

class U_find {
public:
    U_find() {
        parent = NULL;
        Rank = NULL;
        size = 0;
    }
    U_find(int _size) {
        size = _size;
        parent = new int [size];
        Rank = new int [size];
        for(int i = 0;i < size;i++) {
            parent[i] = i;
            Rank[i] = 1;    //初始化深度为1
        }
    }
    ~U_find() {
        if(parent != NULL) {
            delete [] parent;
            parent = NULL;
            size = 0;
        }
        if(Rank != NULL) {
            delete [] Rank;
            Rank = NULL;
            size = 0;
        }
    }
public:
    //获得并查集元素个数
    int getSize();
    //判断a与b是是否连接
    bool Connected(int a,int b);
    //合并a与b所在集合
    void Union(int a,int b);
private:
    //查找a所在的根节点
    int find(int a);
private:
    int  *parent;
    int * Rank; //保存树的深度
    int size;
};
