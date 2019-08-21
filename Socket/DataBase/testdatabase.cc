#include "dataBase.cc"

int main (void) {
    DataBase base(10);
    base.addKeySpace(DataStructure::ObjList,DataStructure::EncodingRaw,"name","cx",3);
    base.addKeySpace(DataStructure::ObjList,DataStructure::EncodingRaw,"gender","m",3);
    sleep(4);
    std::cout << "get messgae: " << base.getKeySpace(DataStructure::ObjList,"name") << std::endl;
    // base.delKeySpace(DataStructure::ObjString,"name");
    std::cout << "get messgae: " << base.getKeySpace(DataStructure::ObjList,"gender") << std::endl;
    return 0;
}
