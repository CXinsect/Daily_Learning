#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <list>
int main () {
    char buf[1024] = {0};
    std::string tmp = getcwd(buf,sizeof(buf));
    tmp += "/1.txt";
    // std::ofstream out (tmp);
    std::ofstream out;
    out.open(tmp,std::ios::out | std::ios::app);
    if(out.is_open()) {
        std::string test = "hello,Linux\r\n";
        out.write(test.c_str(),test.size());
    }
    else std::cout << "No file" << std::endl;
    printf("%s\n",tmp.c_str());
    short a = 2;
    printf("short:%hd\n",a);
    int ret = sprintf(buf,"%dhello",a);
    std::cout << "tmp: " << ret << std::endl;
    out.close();

    std::list<std::string> v;
    v.push_back("cx");
    v.push_back("lala");
    v.push_back("nini");
    v.push_back("haha");
    std::list<std::string>::iterator it = v.begin();
    char buffer[1024] = {0};
    char *ptr = buffer;
    int sum = 0,nwrite = 0;
    while(it != v.end()) {
        nwrite = snprintf(ptr+sum,sizeof(buffer)-sum,"%s",(*it).c_str());
        sum += nwrite;
        it++;
    }
    std::cout << "buffer: "<< buffer << std::endl;
    return 0;
}