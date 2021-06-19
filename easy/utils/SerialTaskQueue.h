

#pragma once

#include "TaskQueue.h"
#include <easy/net/EventLoopThread.h>
#include <easy/Config.hpp>
#include <string>
#include <queue>
#include <mutex>
#include <atomic>
namespace easy
{
/**
 * @brief This class represents a task queue in which all tasks are executed one
 * by one.
 *
 */
class EASY_EXPORT SerialTaskQueue : public TaskQueue
{
  public:
    /**
     * @brief Run a task in the queue.
     *
     * @param task
     */
    virtual void runTaskInQueue(const std::function<void()> &task);
    virtual void runTaskInQueue(std::function<void()> &&task);

    /**
     * @brief Get the name of the queue.
     *
     * @return std::string
     */
    virtual std::string getName() const
    {
        return queueName_;
    };

    /**
     * @brief Wait until all tasks in the queue are finished.
     *
     */
    void waitAllTasksFinished();

    SerialTaskQueue() = delete;

    /**
     * @brief Construct a new serail task queue instance.
     *
     * @param name
     */
    explicit SerialTaskQueue(const std::string &name);

    virtual ~SerialTaskQueue();

    /**
     * @brief Check whether a task is running in the queue.
     *
     * @return true
     * @return false
     */
    bool isRuningTask()
    {
        return loopThread_.getLoop()
                   ? loopThread_.getLoop()->isCallingFunctions()
                   : false;
    }

    /**
     * @brief Get the number of tasks in the queue.
     *
     * @return size_t
     */
    size_t getTaskCount();

    /**
     * @brief Stop the queue.
     *
     */
    void stop();

  protected:
    std::string queueName_;
    EventLoopThread loopThread_;
    bool stop_{false};
};
}  // namespace easy
