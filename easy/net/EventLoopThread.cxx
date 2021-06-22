


#include <easy/net/EventLoopThread.h>
#include <easy/utils/Logger.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif


namespace easy {


EventLoopThread::EventLoopThread(const std::string &threadName, ThreadInitCB const& cb)
    : loop_(nullptr)
    , loopThreadName_(threadName)
    , thread_([this]() { loopFuncs(); })
    , initCB_(cb)
{
    auto f = promiseForLoopPointer_.get_future();
    loop_ = f.get();
}



EventLoopThread::~EventLoopThread()
{
    run();
    if (loop_)
    {
        loop_->quit();
    }
    if (thread_.joinable())
    {
        thread_.join();
    }
}


void EventLoopThread::wait()
{
    thread_.join();
}


void EventLoopThread::loopFuncs()
{
#ifdef __linux__
    ::prctl(PR_SET_NAME, loopThreadName_.c_str());
#endif

    EventLoop loop;

    if(initCB_)
        initCB_(&loop);

    loop.queueInLoop([this]() { promiseForLoop_.set_value(1); });
    promiseForLoopPointer_.set_value(&loop);
    auto f = promiseForRun_.get_future();
    (void)f.get();
    loop.loop();
    // LOG_DEBUG << "loop out";
    loop_ = NULL;
}


void EventLoopThread::run()
{
    std::call_once(once_, [this]() {
        auto f = promiseForLoop_.get_future();
        promiseForRun_.set_value(1);
        // Make sure the event loop loops before returning.
        (void)f.get();
    });
}


} // namespace easy