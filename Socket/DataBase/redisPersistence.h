#ifndef _PERSISTENCE_H_
#define _PERSISTENCE_H_
#include "model.h"
class Persistence {
    public:
    private:
        long long dirty_;
        time_t lastsave_;
};
#endif