#ifndef __FILE_H_
#define __FILE_H_

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <mutex>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <string.h>

using namespace std;

class MmapFile {
    public:
        MmapFile() : isVaild_(false),fd_(0),fileSize_(0),filePtr_(NULL) {}
        MmapFile(int filesize,string filepath) : isVaild_(false),
                                                fd_(0),
                                                fileSize_(filesize),
                                                filePtr_(nullptr),
                                                filePath_(filepath) {
            // MmapOpen();
        }
        void MmapOpen() {
            assert((fd_ = open(filePath_.c_str(),O_RDONLY)) != -1);
            cout << filePath_ <<  " " << fileSize_ << endl;
            filePtr_ = static_cast<char*>(mmap(NULL,fileSize_,PROT_READ,MAP_SHARED,fd_,0));
           
           if(filePtr_ == MAP_FAILED) {
               cout << strerror(errno) << endl;
               assert(filePtr_ != MAP_FAILED);
           }
            isVaild_ = true;
        }
        void MmapClose() {
            if(filePtr_ != NULL) {
                assert(munmap(filePtr_,fileSize_) != -1);
                close(fd_);
                filePtr_ = NULL;
                isVaild_ = false;
            }
        }
        char* getFilePtr() { return filePtr_; }
        bool getIsVaild() { return isVaild_; }
        int getFileSize() { return fileSize_; }
        string getFilePath() { return filePath_; }
        
        static string getCwd() {
            char* buf = NULL;
            int size = 64;
            do {
                buf = new char[size];
                if(getcwd(buf,size) != NULL) break;
                else {
                    size *= 2;
                    delete[] buf;
                }
            } while(true);
            string temp = buf;
            delete[] buf;
            return temp;
        } 

    virtual ~MmapFile() {
        MmapClose();
    }
    private:
        int fd_;
        int fileSize_;
        char* filePtr_;
        string filePath_;
        bool isVaild_;
};


#endif