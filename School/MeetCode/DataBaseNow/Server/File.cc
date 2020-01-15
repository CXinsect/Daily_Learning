// #define _GNU_SOURCE

#include "File.h"

bool fileManager::getFile(int fileID,const string& filePath,int fileSize,unique_ptr<fileSource> source) {
    unique_lock<mutex> mylock(mutex_);
    bool found = false;
    auto it = fileUnused_.begin();

    while(it != fileUnused_.end()) {
        if(it->fileID_ != fileID) it++;
        else if(it->fileSize_ == fileSize) {
            break;
        } else {
            munmap(it->fileMmapPtr_,it->fileSize_);
            close(it->fd_);
            fileUnused_.erase(it);
        }
    }
    if (it != fileUnused_.end()) {
        source.reset(new fileSource(*it));        
        // source->fileID_ = it->fileID_;
        // source->fd_ = it->fd_;
        // source->fileMmapPtr_ = it->fileMmapPtr_;
        // source->fileSize_ = it->fileSize_;
        // source->numReference_ = 1;
        source->numReference_ = 1;
        fileUnused_.erase(it);
        fileUsed_.insert(make_pair(fileID,new fileSource(*source)));
        //todo????
        found = true;
    } else {
        auto range = fileUsed_.equal_range(fileID);
        auto iter = range.first;
        for(;iter != range.second;iter++) {
            if(iter->second->fileSize_ == fileSize) break;
        } 
        if(iter != fileUsed_.end()) {
            iter->second->numReference_ += 1;
            source = std::move(iter->second);
            found = true;
        }
    }
    if(found) return true;
    int fd = open(filePath.c_str(),O_CREAT | O_WRONLY,0644);
    assert(fd != -1);
    char* ptr = static_cast<char*>(mmap(0,fileSize,PROT_READ,MAP_SHARED,fd,0));
    assert(ptr != MAP_FAILED);
    unique_ptr<fileSource> file(new fileSource(fileID,fd,ptr,fileSize,1));
    source = std::move(file);
    fileUsed_.insert(make_pair(fileID,new fileSource(*source)));
    return true;
}

void fileManager::releaseFile(int fileID,int fileSize) {
    unique_lock<mutex> mylock(mutex_);
    auto range = fileUsed_.equal_range(fileID);
    auto it = range.first;
    while(it != range.second) {
        if(it->second->fileSize_ == fileSize) break;
        it++;
    }
    if(it == range.second) return;
    if(it->second->numReference_ > 1) {
        it->second->numReference_ -= 1;
    } else {
        it->second->numReference_ = 0;
        fileUnused_.push_back(*it->second);
        fileUsed_.erase(it);
    }
}