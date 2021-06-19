

#pragma once

#include <easy/net/EventLoop.h>
#include <easy/utils/NonCopyable.h>
#include <easy/Config.hpp>
#include <mutex>
#include <thread>
#include <memory>
#include <condition_variable>
#include <future>

namespace easy
{
/**
 * @brief This class represents an event loop thread.
 *
 */
class EASY_EXPORT EventLoopThread : NonCopyable
{
  public:
    explicit EventLoopThread(const std::string &threadName = "EventLoopThread");
    ~EventLoopThread();

    /**
     * @brief Wait for the event loop to exit.
     * @note This method blocks the current thread until the event loop exits.
     */
    void wait();

    /**
     * @brief Get the pointer of the event loop of the thread.
     *
     * @return EventLoop*
     */
    EventLoop *getLoop() const
    {
        return loop_;
    }

    /**
     * @brief Run the event loop of the thread. This method doesn't block the
     * current thread.
     *
     */
    void run();

  private:
    EventLoop *loop_;
    std::string loopThreadName_;
    void loopFuncs();
    std::promise<EventLoop *> promiseForLoopPointer_;
    std::promise<int> promiseForRun_;
    std::promise<int> promiseForLoop_;
    std::once_flag once_;
    std::thread thread_;
};

}  // namespace easy
