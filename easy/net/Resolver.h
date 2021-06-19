

#pragma once
#include <easy/Config.hpp>
#include <memory>
#include <easy/net/EventLoop.h>
#include <easy/net/InetAddress.h>

namespace easy
{
/**
 * @brief This class represents an asynchronous DNS resolver.
 * @note Although the c-ares library is not essential, it is recommended to
 * install it for higher performance
 */
class EASY_EXPORT Resolver
{
  public:
    using Callback = std::function<void(const easy::InetAddress&)>;

    /**
     * @brief Create a new DNS resolver.
     *
     * @param loop The event loop in which the DNS resolver runs.
     * @param timeout The timeout in seconds for DNS.
     * @return std::shared_ptr<Resolver>
     */
    static std::shared_ptr<Resolver> newResolver(EventLoop* loop = nullptr,
                                                 size_t timeout = 60);

    /**
     * @brief Resolve an address asynchronously.
     *
     * @param hostname
     * @param callback
     */
    virtual void resolve(const std::string& hostname,
                         const Callback& callback) = 0;

    virtual ~Resolver()
    {
    }

    /**
     * @brief Check whether the c-ares library is used.
     *
     * @return true
     * @return false
     */
    static bool isCAresUsed();
};
}  // namespace easy
