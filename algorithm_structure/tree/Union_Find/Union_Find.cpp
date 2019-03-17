#include "Union_Find.h"

int U_find::getSize() {
    return size;
}
int U_find::find(int a) {
    if(a < 0 || a >= size) {
        cout<<"The index is Wrong "<<endl;
        return -1;
    }
    while(a != parent[a]) {
        //将所传节点的父亲节点指定为当前父亲节点的父亲节点
        parent[a] = parent[parent[a]];
        a = parent[a];
    }
    return a;
}
bool U_find::Connected(int a,int b) {
    return find(a) == find(b);
}
void U_find::Union(int a,int b) {
    int aroot = find(a);
    int broot = find(b);
    if(aroot == broot) {
        parent[a] = b;
    }
    else if(Rank[a] > Rank[b]) {
        parent[b] = a;
    }
    else if(Rank[a] < Rank[b]) {
        parent[a] = b;
    }
    else {
        parent[a] = b;
        Rank[b] += 1;
    }
}
