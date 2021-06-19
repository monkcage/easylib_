

#pragma once
#include <easy/net/Resolver.h>
#include <easy/utils/NonCopyable.h>
#include <easy/utils/ConcurrentTaskQueue.h>
#include <memory>
#include <vector>
#include <thread>

namespace easy
{
constexpr size_t kResolveBufferLength{16 * 1024};
class NormalResolver : public Resolver,
                       public NonCopyable,
                       public std::enable_shared_from_this<NormalResolver>
{
  public:
    virtual void resolve(const std::string& hostname,
                         const Callback& callback) override;
    explicit NormalResolver(size_t timeout)
        : timeout_(timeout), resolveBuffer_(kResolveBufferLength)
    {
    }
    virtual ~NormalResolver()
    {
    }

  private:
    static std::unordered_map<std::string,
                              std::pair<easy::InetAddress, easy::Date>>&
    globalCache()
    {
        static std::unordered_map<
            std::string,
            std::pair<easy::InetAddress, easy::Date>>
            dnsCache_;
        return dnsCache_;
    }
    static std::mutex& globalMutex()
    {
        static std::mutex mutex_;
        return mutex_;
    }
    static easy::ConcurrentTaskQueue& concurrentTaskQueue()
    {
        static easy::ConcurrentTaskQueue queue(
            std::thread::hardware_concurrency() < 8
                ? 8
                : std::thread::hardware_concurrency(),
            "Dns Queue");
        return queue;
    }
    const size_t timeout_;
    std::vector<char> resolveBuffer_;
};
}  // namespace easy
