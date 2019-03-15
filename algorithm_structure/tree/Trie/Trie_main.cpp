#include "Trie.cpp"

using namespace std;

int main()
{
    Trie t;
    string c("hello");
    string a("he");
    t.add(c,3);
    t.add(a,2);
    if(t.contains("he")) {
        cout<<"exist"<<endl;
    }
    else {
        cout<<"no exist"<<endl;
    }
    int ret = t.prefix_nums(a);
    cout<<"ret: "<<ret<<endl;
    return 0;
}

