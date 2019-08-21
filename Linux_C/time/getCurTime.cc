#include <time.h>
#include <iostream>
#include <string>
#include <assert.h>

std::string getCurTime(const char *format) {
    std::string buf = std::string();
    time_t t = time(NULL);
    struct tm *locp = localtime(&t);
    if(locp == NULL) {
        std::cout << "Time Got Error" << std::endl;
        exit(EXIT_FAILURE);
    }
    struct tm loc = *locp;
    size_t s = strftime(const_cast<char*>(buf.c_str()),1024,(format != NULL) ? format : "%c",&loc);
    if(s == 0) {
        std::cout << "strftime Error " << std::endl;
        exit(EXIT_FAILURE);
    }
    return buf.c_str();
}