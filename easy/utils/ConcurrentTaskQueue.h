

#pragma once

#include <easy/utils/TaskQueue.h>
#include <easy/Config.hpp>
#include <list>
#include <memory>
#include <vector>
#include <queue>
#include <string>

namespace easy
{
/**
 * @brief This class implements a task queue running in parallel. Basically this
 * can be called a threads pool.
 *
 */
class EASY_EXPORT ConcurrentTaskQueue : public TaskQueue
{
  public:
    /**
     * @brief Construct a new concurrent task queue instance.
     *
     * @param threadNum The number of threads in the queue.
     * @param name The name of the queue.
     */
    ConcurrentTaskQueue(size_t threadNum, const std::string &name);

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
     * @brief Get the number of tasks to be executed in the queue.
     *
     * @return size_t
     */
    size_t getTaskCount();

    /**
     * @brief Stop all threads in the queue.
     *
     */
    void stop();

    ~ConcurrentTaskQueue();

  private:
    size_t queueCount_;
    std::string queueName_;

    std::queue<std::function<void()>> taskQueue_;
    std::vector<std::thread> threads_;

    std::mutex taskMutex_;
    std::condition_variable taskCond_;
    std::atomic_bool stop_;
    void queueFunc(int queueNum);
};

}  // namespace easy
