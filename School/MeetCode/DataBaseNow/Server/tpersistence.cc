#include "redisPersistence.h"
#include "dataBase.h"

#include <unistd.h>
int main (void) {
    ThreadPool* tp = new ThreadPool(2);
    std::shared_ptr<DataBase> data(new DataBase(1));
    data->addKeySpace(DataStructure::ObjString,DataStructure::EncodingRaw,"name","cxdd",DataStructure::SpareTire,11);
    data->addKeySpace(DataStructure::ObjString,DataStructure::EncodingRaw,"gender","cxdd",DataStructure::SpareTire,11);
//    data->addKeySpace(DataStructure::ObjList,DataStructure::EncodingRaw,"gender","cx",DataStructure::SpareTire,11);
//    data->addKeySpace(DataStructure::ObjList,DataStructure::EncodingRaw,"gender","nini",DataStructure::SpareTire,11);
//    data->addKeySpace(DataStructure::ObjList,DataStructure::EncodingRaw,"gender","haha",DataStructure::SpareTire,11);
//    data->addKeySpace(DataStructure::ObjHash,DataStructure::EncodingRaw,"dot","color","white",11);
//    data->addKeySpace(DataStructure::ObjHash,DataStructure::EncodingRaw,"dot","color","black",11);
//    data->addKeySpace(DataStructure::ObjHash,DataStructure::EncodingRaw,"dot","name","cx",11);
//    data->addKeySpace(DataStructure::ObjHash,DataStructure::EncodingRaw,"dot","gender","m",11);


    
    // std::map<std::pair<std::string, long long>,std::string> ::iterator it = data->getKeySpaceStringObject().begin();
    // while(it != data->getKeySpaceStringObject().end()) {cout << it->second << endl; it++; }
    tp->Start();
    this_thread::sleep_for(chrono::milliseconds(50));   
    threadStorage* t = new threadStorage(data,1);
    tp->addTask(t);
    tp->addTask(t);
    // t->Close();
    tp->Stop();
    cout << "hello" << endl;
}