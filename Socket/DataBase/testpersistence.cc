// #include "dataBase.h"
#include "redisPersistence.cc"

int main (void) {
    std::shared_ptr<DataBase> data(new DataBase(1));
    data->addKeySpace(DataStructure::ObjString,DataStructure::EncodingRaw,"name","cx",DataStructure::SpareTire,11);
    data->addKeySpace(DataStructure::ObjString,DataStructure::EncodingRaw,"gender","cx",DataStructure::SpareTire,11);
    data->addKeySpace(DataStructure::ObjList,DataStructure::EncodingRaw,"gender","cx",DataStructure::SpareTire,11);
    data->addKeySpace(DataStructure::ObjList,DataStructure::EncodingRaw,"gender","nini",DataStructure::SpareTire,11);
    data->addKeySpace(DataStructure::ObjList,DataStructure::EncodingRaw,"gender","haha",DataStructure::SpareTire,11);
    data->addKeySpace(DataStructure::ObjHash,DataStructure::EncodingRaw,"dot","color","white",11);
    data->addKeySpace(DataStructure::ObjHash,DataStructure::EncodingRaw,"dot","color","black",11);
    data->addKeySpace(DataStructure::ObjHash,DataStructure::EncodingRaw,"dot","name","cx",11);
    data->addKeySpace(DataStructure::ObjHash,DataStructure::EncodingRaw,"dot","gender","m",11);


    
    std::map<std::pair<std::string, long long>,std::multimap<std::string, std::string>> ::iterator it = data->getKeySpaceHashObject().begin();
    std::multimap<std::string, std::string> iter = it->second;
    std::cout << "map" << iter.size() << std::endl;
    Persistence filesave(data);
    filesave.CheckStorageConditions();
}