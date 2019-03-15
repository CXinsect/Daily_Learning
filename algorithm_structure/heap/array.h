#pragma once
#include <iostream>
class array
{
public:
    array()
    {
        data = NULL;
        arg = NULL;
    }
    array(int capicaty) 
    {
        this->capicaty = capicaty;
        size = 0;
        data = new int [capicaty];
    }
     ~array() 
     {
        if(data != NULL)
        {
            delete [] data;
            size = 0;
            capicaty = 0;
        }
    }
public:
    void addHead(int el);
    void addTail(int el);
    void add(int index,int);
    int find (int el);
    void set (int index,int el);
    int get(int index);
    int deleteHead();
    int deleteTail();
    int  Delete(int);
    void remove(int);
    bool IsEmpty();
    bool IsFull();
    int getSize();
    int getCapicaty();
    void Print();
    int getHead();
    int getTail();
    void Swap(int index,int t);

public:
    array & operator = (array & a);
     void reSize(int newCapicaty,int * arg);

private:
    int * data;
    int size;
    int capicaty;
public:
    int * arg;
};

