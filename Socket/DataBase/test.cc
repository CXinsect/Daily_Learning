#include "dataBase.cc"

int main (void) {
    DataBase base(10);
    base.addKeySpace(DataStructure::ObjString,DataStructure::EncodingRaw,"name","cx",123);
    base.addKeySpace(DataStructure::ObjString,DataStructure::EncodingRaw,"gender","m",1234);
    std::cout << "get messgae: " << base.getKeySpace(DataStructure::ObjString,"name") << std::endl;
    base.delKeySpace(DataStructure::ObjString,"name");
    std::cout << "get messgae: " << base.getKeySpace(DataStructure::ObjString,"name") << std::endl;
    return 0;
}
