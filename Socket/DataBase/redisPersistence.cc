#include "redisPersistence.h"

bool Persistence::rdbSave (const std::string filename) {
    struct sigaction act , act1;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGUSR2);
    sigprocmask(SIG_BLOCK,&set,NULL);
    pid_t pid = fork();
    son_pid_ = pid;
    if(pid > 0) {
        char buf[1024] = {0};
        std::string tmp = getcwd(buf,sizeof(buf));
        assert(tmp.c_str() != NULL);
        std::cout << "tmp: The File Path: " << tmp << std::endl;
        std::ofstream out(tmp);
        //如果字符串对象不为空便开始写入过程
        if(database_->getKeySpaceListObject().size() != 0) {
        }
    }
}