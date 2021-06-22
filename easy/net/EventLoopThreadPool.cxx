
#include <easy/net/EventLoopThreadPool.h>
using namespace easy;
EventLoopThreadPool::EventLoopThreadPool(size_t threadNum,
                                         const std::string &name)
    : loopIndex_(0)
    , name_(name)
    , threadNum_(threadNum)
{
    // for (size_t i = 0; i < threadNum; ++i)
    // {
    //     loopThreadVector_.emplace_back(std::make_shared<EventLoopThread>(name));
    // }
}


void EventLoopThreadPool::start(ThreadInitCB const& cb)
{
    for (unsigned int i = 0; i < loopThreadVector_.size(); ++i)
    {
        loopThreadVector_.emplace_back(std::make_shared<EventLoopThread>(name_, cb));
        loopThreadVector_[i]->run();
    }
}


void EventLoopThreadPool::wait()
{
    for (unsigned int i = 0; i < loopThreadVector_.size(); ++i)
    {
        loopThreadVector_[i]->wait();
    }
}
EventLoop *EventLoopThreadPool::getNextLoop()
{
    if (loopThreadVector_.size() > 0)
    {
        EventLoop *loop = loopThreadVector_[loopIndex_]->getLoop();
        ++loopIndex_;
        if (loopIndex_ >= loopThreadVector_.size())
            loopIndex_ = 0;
        return loop;
    }
    return nullptr;
}
EventLoop *EventLoopThreadPool::getLoop(size_t id)
{
    if (id < loopThreadVector_.size())
        return loopThreadVector_[id]->getLoop();
    return nullptr;
}
std::vector<EventLoop *> EventLoopThreadPool::getLoops() const
{
    std::vector<EventLoop *> ret;
    for (auto &loopThread : loopThreadVector_)
    {
        ret.push_back(loopThread->getLoop());
    }
    return ret;
}
