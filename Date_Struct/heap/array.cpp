#include <iostream>
#include "array.h"
using namespace std;
bool array::IsEmpty()
{
    return size == 0;
}
bool array::IsFull()
{
    return size == getCapicaty();
}
int array::getSize()
{
    return size;
}
int array::getCapicaty()
{
    return capicaty;
}
void array::addHead(int el)
{
    add(0,el);
}
void array::addTail(int el)
{
    add(size,el);
}
void array::add(int index,int el)
{
    if(index < 0 || index > getSize())
    { cout<<"索引错误: "<<endl;
        return ;
    }
    else
    {
        for(int i = size - 1; i>= index; i--)
            data[i+1] = data[i];
        data[index] = el;
         if(IsFull())
        {
            reSize(capicaty * 2,arg);
        }
        size++;
    }
}
int array::find(int el)
{
    int i = 0,info = -1;
    for(i = 0;i<getSize();i++)
    {
        if(data[i] == el)
            info = i;
    }
    if(i > size)
    {
        cout<<"No Element: "<<endl;
        return -1;
    }
    return info;
}
int array::deleteHead()
{
    return Delete(0);
}
int array::deleteTail()
{
    return Delete(size-1);
}
int array::Delete(int index)
{
    int info = data[index];
    if(index < 0 || index > size)
    {
        cout<<"index error"<<endl;
        return -1;
    }
        cout<<"size: "<<size<<"capicaty: "<<capicaty<<endl;
        for(int i = index;i<getSize();i++) {
            if(i == getCapicaty()) {
                size--;
                break;
            }
            data[i] = data[i+1];
        }
        if(size == capicaty /  4  && capicaty / 2  > 0)
            reSize(capicaty / 2,arg);
        size--;
    return info;
}
void array::remove(int el)
{
    int index = find(el);
    if(index < 0 || index > getSize())
    { 
        cout<<"remove索引错误: "<<endl;
        cout<<"index"<<index<<endl;
        return ;
    }
    Delete(index);
}
int array::get(int index)
{
    if(index < 0 || index > getSize())
    {
        cout<<"error index"<<endl;
        return -1;
    }
    return data[index];
}
int array::getHead()
{
  return get(0);  
}
int array::getTail()
{
    return get(size - 1);
}
void array::set(int index,int el)
{
    if(index < 0 || index > getSize())
    {
        cout<<"error index"<<endl;
        return;
    }
    data[index] = el;
}
void array::Print()
{
    for(int i = 0;i<getSize();i++)
        cout<<data[i]<<" ";
    cout<<endl;
}
void array::reSize(int newCapicaty,int *Newdata)
{
    cout<<"dog"<<endl;
    Newdata = new int [newCapicaty];
    cout<<"capiacty"<<newCapicaty<<endl;
    for(int i = 0;i<size;i++)
    {
        Newdata[i] = data[i];
    }
    if(data != NULL) {
        delete [] data;
        data = NULL;
        size = 0;
        capicaty = 0;
    }
    data = Newdata;
    Newdata = NULL;
    size = getSize();
    capicaty = newCapicaty;
    return;
}
array &  array::operator=(array & a)
{
    if(data != NULL)
    {
        delete [] data;
        size = 0;
        capicaty = 0;
    }
    data = new int [a.capicaty];
    size = a.size;
    capicaty = a.capicaty;
    for(int i = 0;i<a.size;i++)
    {
        data[i] = a.data[i];
    }
    return *this;
}
void array::Swap(int index,int t) {
    if(index < 0 || index > getSize() || t < 0 || t > getSize()) {
        cout<<"The index is wrong Swap in Array"<<endl;
        return;
    }
    else {
        int temp = data[index];
        data[index] = data[t];
        data[t] = temp;
    }
}
