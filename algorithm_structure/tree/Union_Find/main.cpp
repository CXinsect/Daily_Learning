#include "Union_Find.cpp"

int main()
{
    U_find u(5);
    cout<<"size: "<<u.getSize()<<endl;
    if(u.Connected(1,2)) {
        cout<<"Connected"<<endl;
    }
    else {
        cout<<"Disconnected"<<endl;
    }
    u.Union(1,2);
    if(u.Connected(1,2)) {
        cout<<"ED Connected"<<endl;
    }
    return 0;
}

