

#include <easy/utils/SerialTaskQueue.h>
#include <easy/utils/Logger.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif
namespace easy
{


SerialTaskQueue::SerialTaskQueue(const std::string &name)
    : queueName_(name.empty() ? "SerailTaskQueue" : name),
      loopThread_(queueName_)
{
    loopThread_.run();
}


void SerialTaskQueue::stop()
{
    stop_ = true;
    loopThread_.getLoop()->quit();
    loopThread_.wait();
}


SerialTaskQueue::~SerialTaskQueue()
{
    if (!stop_)
        stop();
    LOG_TRACE << "destruct SerialTaskQueue('" << queueName_ << "')";
}


void SerialTaskQueue::runTaskInQueue(const std::function<void()> &task)
{
    loopThread_.getLoop()->runInLoop(task);
}


void SerialTaskQueue::runTaskInQueue(std::function<void()> &&task)
{
    loopThread_.getLoop()->runInLoop(std::move(task));
}


void SerialTaskQueue::waitAllTasksFinished()
{
    syncTaskInQueue([]() {
    });
}



}  // namespace easy
