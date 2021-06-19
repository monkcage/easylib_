

#pragma once

#include <easy/net/EventLoopThread.h>
#include <easy/Config.hpp>
#include <vector>
#include <memory>

namespace easy
{
/**
 * @brief This class represents a pool of EventLoopThread objects
 *
 */
class EASY_EXPORT EventLoopThreadPool : NonCopyable
{
  public:
    EventLoopThreadPool() = delete;

    /**
     * @brief Construct a new event loop thread pool instance.
     *
     * @param threadNum The number of threads
     * @param name The name of the EventLoopThreadPool object.
     */
    EventLoopThreadPool(size_t threadNum,
                        const std::string &name = "EventLoopThreadPool");

    /**
     * @brief Run all event loops in the pool.
     * @note This function doesn't block the current thread.
     */
    void start();

    /**
     * @brief Wait for all event loops in the pool to quit.
     *
     * @note This function blocks the current thread.
     */
    void wait();

    /**
     * @brief Return the number of the event loop.
     *
     * @return size_t
     */
    size_t size()
    {
        return loopThreadVector_.size();
    }

    /**
     * @brief Get the next event loop in the pool.
     *
     * @return EventLoop*
     */
    EventLoop *getNextLoop();

    /**
     * @brief Get the event loop in the `id` position in the pool.
     *
     * @param id The id of the first event loop is zero. If the id >= the number
     * of event loops, nullptr is returned.
     * @return EventLoop*
     */
    EventLoop *getLoop(size_t id);

    /**
     * @brief Get all event loops in the pool.
     *
     * @return std::vector<EventLoop *>
     */
    std::vector<EventLoop *> getLoops() const;

  private:
    std::vector<std::shared_ptr<EventLoopThread>> loopThreadVector_;
    size_t loopIndex_;
};
}  // namespace easy
