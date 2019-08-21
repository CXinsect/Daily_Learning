#include <locale.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <assert.h>
#include <memory>
#include <iostream>
const long long SecondsInaYear = 365.24219 *24*60*60;

int main () {
    time_t t = time(NULL);
    printf("senconds since the Epoch: %ld\n",(long)t);

    struct timeval tv;
    assert(gettimeofday(&tv,NULL) != -1);
    printf("gettimeofday return: %ld senconds %ld microsecs\n",(long)tv.tv_sec,(long)tv.tv_usec);
    // std::shared_ptr <struct tm> gtm;
    struct tm *gmp = gmtime(&t);
    assert(gmp != NULL);
    std::shared_ptr <struct tm> tgmp(gmp);
    struct tm gm = *tgmp.get();
    std::cout << "Broken down By gmtime()" << std::endl;
    printf("seconds:%d min:%d,hour: %d day: %d month:%d\n",gm.tm_sec,gm.tm_min,gm.tm_hour,gm.tm_mday,gm.tm_mon);
    struct tm *locp = localtime(&t);
    assert(locp);
    struct tm loc = *locp;
    std::cout << "Broken down localTime" << std::endl;
    printf("seconds:%d min:%d,hour: %d day: %d month:%d\n",gm.tm_sec,gm.tm_min,gm.tm_hour,gm.tm_mday,gm.tm_mon);
    std::cout << "Time Print" << std::endl;
    printf("acstime() formats the gettime: %s\n",asctime(&gm));
    printf("ctime() formats the ctime: %s\n",ctime(&t));
    printf("mktime() of gettime() value: %ld\n",(long)mktime(&gm));
    printf("mktime() of localetime() value: %ld\n",(long)mktime(&loc));
    exit(EXIT_SUCCESS);    
}