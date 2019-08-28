#include "Epoll.h"

void Epoll::epoll(ChannelList *activeChannel) {
    int numEvents = ::epoll_wait(epollfd_, &*events_.begin(),static_cast<int>(events_.size()),-1);
    std::cout << strerror(errno) << std::endl;
    // assert(numEvents > 0);
    if(numEvents > 0) {
        std::cout << "Events Happened: " << numEvents << std::endl;
        fillActiveChannel(numEvents,activeChannel);
        //Expand if there is not enough space
        if(numEvents == events_.size()) {
            events_.resize(events_.size() *2);
        }
    }
    else if(numEvents == 0) 
        std::cout << "No things Happened" << std::endl;
    else 
        abort();
    return;
}
void Epoll::fillActiveChannel(int numEvents, ChannelList *activeChannelList) {
    assert(numEvents <= events_.size());
    for(int i = 0;i < numEvents; i++) {
        Channel *channel = static_cast<Channel*>(events_[i].data.ptr);
        int sockfd = channel->getSockfd();
        ChannelMap::const_iterator it = channel_.find(sockfd);
        assert(it != channel_.end());
        assert(channel == it->second);
        channel->setRevents(events_[i].events);
        activeChannelList->push_back(channel);
    }
}

void Epoll::updateChannel(Channel *channel) {
    // ChannelMap::const_iterator it = channel_.find(channel->getSockfd());
    if(channel->getIndex() < 0) {
        struct epoll_event efd;
        efd.events = channel->getEvents();
        efd.data.fd = channel->getSockfd();
        efd.data.ptr = channel;
        int ret = ::epoll_ctl(epollfd_,EPOLL_CTL_ADD,channel->getSockfd(),&efd);
        assert(ret == 0);
        events_.push_back(efd);
        channel->setIndex((int)events_.size()-1);
        channel_[channel->getSockfd()] = channel;
    }
    else {
        //channel has been existed , update
        assert(channel_.find(channel->getSockfd()) != channel_.end());
        assert(channel_[channel->getSockfd()] == channel);
        int index = channel->getIndex();
        assert(index >= 0 && index <= (int)events_.size());
        struct epoll_event &efd = events_[index];
        assert(efd.data.fd == channel->getSockfd() || efd.data.fd == -1);
        efd.events = channel->getEvents();
        // efd.data.ptr = channel;
        if(channel->isNoneEvent())
            efd.data.fd = -1;
    }
}