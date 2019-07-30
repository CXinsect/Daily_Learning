#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop *loop) : owerLoop_(loop) {}

Poller::~Poller() {};

void Poller::poll(int timeoutMs,ChannelList *activeChannels) {
    int numEvents = ::poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);

    if(numEvents > 0) {
        std::cout << "active Channels" << numEvents << std::endl;
        fillActiveChannels(numEvents,activeChannels);
    }
    else if(numEvents == 0) {
        std::cout << "Nothing happend" << std::endl;
    }
    else {
        std::cout << __FILE__ << ": " << __LINE__ << std::endl;
        exit(0);
    }
    return;
}
void Poller::fillActiveChannels (int numEvents,ChannelList *activeChannels) const {
    for(PollList::const_iterator pfd = pollfds_.begin();
        pfd != pollfds_.end() && numEvents > 0;++pfd) {
            if(pfd->revents > 0) {
                --numEvents;
                ChannelMap::const_iterator ch = channels_.find(pfd->fd);
                assert(ch != channels_.end());
                Channel * channel = ch->second;
                assert(channel->getFd() == pfd->fd);
                activeChannels->push_back(channel);
            }
        }
}

void Poller::updateChannel(Channel *channel) {
    std::cout << "fd= " << channel->getFd() << "events= " << channel->getEvents() << std::endl;
    //如果是新事件则加入事件列表
    if(channel->getEvents() > 0) {
        assert(channels_.find(channel->getFd()) == channels_.end());
        struct pollfd pfd;
        pfd.fd = channel->getFd();
        pfd.events = static_cast<short>(channel->getEvents());
        pfd.revents = 0;
        pollfds_.push_back(pfd);
        int id = static_cast <int>(pollfds_.size()) -1;
        channel->setIndex(id);
        channels_[pfd.fd] = channel;
    }
    else {
        //否则则更新描述符信息
        assert(channels_.find(channel->getFd()) != channels_.end());
        assert(channels_[channel->getFd()] == channel);
        int id = channel->getIndex();
        assert(id >= 0 && id < static_cast<int>(pollfds_.size()));
        struct pollfd & pfd = pollfds_[id];
        assert(channel->getFd() == pfd.fd);
        pfd.events = static_cast<short>(channel->getEvents());
        pfd.revents = 0;
        if(channel->isNoneEvent()) 
            pfd.fd = -1; 
    }
}
