#include "threadpoll.h"

template <typename T>

ThreadPool <T> ::ThreadPool(int pthread,int max_pthread) : 
    num_pthread(pthread),num_max_pthread(max_pthread),w_queue(NULL),
    stop(false) {
    
        if(pthread <= 0 || max_pthread <= 0)
            cout<<"Error Pragram"<<endl;
        threads = new pthread_t [max_pthread];
        if(!threads) {
            cout<<"Memory Error"<<endl;
            throw exception();
        }
        for(int i = 0; i < num_pthread;i++) {
            if(pthread_create(threads[i],NULL,worker,this)) {
                delete [] threads;
                throw exception();
            }
            if(pthread_detach(pthread_self())) {
                delete [] threads;
                throw exception();
            }
        }
}
template <typename T> 
ThreadPool <T>::~ThreadPool() {
    delete [] threads;
    stop = true;
}
