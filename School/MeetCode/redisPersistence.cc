#include "redisPersistence.h"
void Persistence::parentHandle(int sig) {
std::cout << "parent process" << std::endl;
}
bool Persistence::CheckStorageConditions() {
    long save_interval = getTimestamp() - lastsave_;
    if(save_interval > DataStructure::saveTime) {
        rdbSave();
        //update saved time
        lastsave_ = getTimestamp();
        return true;
    }
    return false;
}
void Persistence::rdbSave () {
    struct sigaction act , act1;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGUSR1);
    sigprocmask(SIG_BLOCK,&set,NULL);
    pid_t pid = fork();
    son_pid_ = pid;
    if(pid > 0) {
        //Receive signals from child processes
        do {
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            act.sa_handler = Persistence::parentHandle;
            int ret = sigaction(SIGUSR1,&act,NULL);
            assert(ret != -1);
            ret = sigprocmask(SIG_UNBLOCK,&set,NULL);
            assert(ret == 0);
            sigStop_ = true;
        }while(!sigStop_);
            
    } else if(pid == 0) {
        if(!database_->getKeySpaceStringObject().size() &&
           !database_->getKeySpaceHashObject().size() &&
           !database_->getKeySpaceListObject().size()) {
                std::cout << "The Data is Empty" << std::endl;
                return;
        }
        std::cout << "hello child" << std::endl;
        char buf[1024] = {0};
        std::string tmp = getcwd(buf,sizeof(buf));
        assert(tmp.c_str() != NULL);
        tmp += "/1.rdb";
        std::cout << "tmp: The File Path: " << tmp << std::endl;
        std::ofstream out;
        //首先打开文件
        out.open(tmp,std::ios::app | std::ios::out);

        //如果字符串对象不为空便开始写入过程
        if(database_->getKeySpaceStringObject().size() != 0) {
           //首先判断字符串对象，实现对字符串对象的保存。
            StringIterator_ it = database_->getKeySpaceStringObject().begin();
            while(it != database_->getKeySpaceStringObject().end()) {   
                snprintf(buf,sizeof(buf),"%s%s%ld%s%lld^%hd!%d@%s!%d$%s%d%ld\r\n",FixedStructure.c_str(),Database.c_str(),databaseNum_,
                                ExpireTime.c_str(),it->first.second,RedisRdbTypeString,(int)it->first.first.size(),
                                it->first.first.c_str(),(int)it->second.size(),it->second.c_str(),Eof,CheckSum);
                std::cout << "Rdb Structure String: " << buf << std::endl;
                out.write(buf,strlen(buf));
                memset(buf,0,sizeof(buf));
                it++;
            }
        }
        if(database_->getKeySpaceHashObject().size() != 0){
            //保存哈希对象
            HashIterator_ it = database_->getKeySpaceHashObject().begin();
            memset(buf,0,sizeof(buf));
            while(it != database_->getKeySpaceHashObject().end()) {
                std::multimap<std::string, std::string>::iterator iter = it->second.begin();
                char tmp[4*1024] = {0};
                char *ptr = tmp;
                int nwrite = 0,sum = 0;
                while(iter != it->second.end()) {
                    nwrite = snprintf(ptr+sum,sizeof(tmp)-sum,"!%d@%s!%d$%s",(int)iter->first.size(),iter->first.c_str(),(int)iter->second.size(),iter->second.c_str());
                    sum += nwrite;
                    iter++;
                }
                std::cout << "tmpbufHash: " << tmp << std::endl;
                snprintf(buf,sizeof(buf),"%s%s%ld%s%lld^%hd!%d#%s!%d%s%d%ld\r\n",FixedStructure.c_str(),Database.c_str(),
                                        databaseNum_,ExpireTime.c_str(),it->first.second,RedisRdbTypeHash,(int)it->first.first.size(),
                                        it->first.first.c_str(),(int)it->second.size(),tmp,Eof,CheckSum);

                std::cout << "Hash Object: " << buf << std::endl;
                out.write(buf,strlen(buf));
                memset(buf,0,sizeof(buf));
                it++;
            }
        }
        if(database_->getKeySpaceListObject().size() != 0) {
           //保存列表对象
            memset(buf,0,sizeof(buf));
            ListIteator_ it = database_->getKeySpaceListObject().begin();
            while(it != database_->getKeySpaceListObject().end()) {
                std::list<std::string>::iterator iter = it->second.begin();
                char tmp[4*1024] = {0};
                char *ptr = tmp;
                int nwrite = 0,sum = 0;
                while(iter != it->second.end()) {
                    nwrite = snprintf(ptr+sum,sizeof(tmp)-nwrite,"!%d$%s",(int)((*iter).size()),(*iter).c_str());
                    sum += nwrite;
                    iter++;
                }
                snprintf(buf,sizeof(buf),"%s%s%ld%s%lld^%hd!%d@%s!%d%s%d%ld\r\n",FixedStructure.c_str(),Database.c_str(),
                                         databaseNum_,ExpireTime.c_str(),it->first.second,RedisRdbTypeList,(int)it->first.first.size(),
                                         it->first.first.c_str(),(int)it->second.size(),tmp,Eof,CheckSum);
                std::cout << "List Object: " << buf << std::endl;
                out.write(buf,strlen(buf));
                memset(buf,0,sizeof(buf));
                it++;
            }
        }
        //Signal the parent process
         kill(getppid(),SIGUSR1);
    }
    //wait for child process
        int ret = waitpid(-1,NULL,WNOHANG);
        assert(ret != -1);
}