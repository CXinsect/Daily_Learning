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

class File {
    public:
        static void filePathAlloc (string path,int length) {
            int fd = open(path.c_str(),O_CREAT | O_WRONLY,0644);
            assert(fd != -1);
            assert(fallocate(fd,0,0,length) != -1);
            close(fd);
        }
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
        static int freeSpace (const char* filePath) {
            struct statvfs sv;
            assert(statvfs(filePath,&sv) != 0);
            if(sv.f_frsize) {
               return sv.f_frsize * sv.f_bavail; 
            } else {
                return sv.f_bsize * sv.f_bavail; 
            }
        }
        static void removeFiles(const string filePath,const string prefix) {
            DIR* directory = opendir(filePath.c_str());
            assert(directory != NULL);
            struct dirent* entry;
            char path[4096] = {0};
            struct stat st;
            while((entry = readdir(directory)) != NULL) {
                int ret = snprintf(path,4096,"%s/%s",filePath.c_str(),entry->d_name);
                if(ret < 0 || ret >= 4096) {
                    cout << "file name is error" << endl;
                    return;
                }
                if(stat(filePath.c_str(),&st) != 0 || !(st.st_mode & S_IFREG) || 
                    strncmp(entry->d_name,prefix.c_str(),prefix.size()) != 0)
                    continue; 
                assert(remove(entry->d_name) != -1);
            }
            closedir(directory);
            return;
        } 
    private:

};

class MmapFile {
    public:
        MmapFile() : isVaild_(false),fd_(0),fileSize_(0),filePtr_(NULL) {}
        MmapFile(int filesize,string filepath) : isVaild_(false),
                                                fd_(0),
                                                fileSize_(filesize),
                                                filePtr_(nullptr),
                                                filePath_(filepath) {
            MmapOpen();
        }
        void MmapOpen() {
            assert((fd_ = open(filePath_.c_str(),O_RDONLY)) != -1);
            filePtr_ = static_cast<char*>(mmap(NULL,fileSize_,PROT_READ,MAP_SHARED,fd_,0));
            assert(filePtr_ != NULL);
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
        bool getFileSize() { return fileSize_; }
        string getFilePath() { return filePath_; }

    ~MmapFile() {
        MmapClose();
    }
    private:
        int fd_;
        int fileSize_;
        char* filePtr_;
        string filePath_;
        bool isVaild_;
};


struct fileSource {
            int fileID_;
            int fd_;
            char* fileMmapPtr_;
            int fileSize_;
            int numReference_;
            fileSource(int fileID,int fd,char* fileMmapPtr,int fileSize,int numReference) {
                this->fileID_ = fileID;
                this->fd_ = fd;
                this->fileMmapPtr_ = fileMmapPtr;
                this->fileSize_ = fileSize;
                this->numReference_ = numReference;
            }
            fileSource& operator=(fileSource file) {
                this->fileID_ = file.fileID_;
                this->fd_ = file.fd_;
                this->fileMmapPtr_ = file.fileMmapPtr_;
                this->fileSize_ = file.fileSize_;
                this->numReference_ = file.numReference_;
                return *this;
            }
        };

class fileManager {
    public:
        
        bool getFile(int fileID,const string& filePath,int fileSize,unique_ptr<fileSource> source);
        void releaseFile(int fileId,int fileSize);
        int getNumFile() { return fileUnused_.size() + fileUsed_.size(); }
    private:
        
        mutex mutex_;
        vector<fileSource> fileUnused_;
        multimap<int,unique_ptr<fileSource>> fileUsed_;

};
#endif