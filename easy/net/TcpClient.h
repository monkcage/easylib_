
#pragma once
#include <easy/net/EventLoop.h>
#include <easy/net/InetAddress.h>
#include <easy/net/TcpConnection.h>
#include <easy/Config.hpp>
#include <functional>
#include <thread>
#include <atomic>
#include <signal.h>
namespace easy
{
class Connector;
using ConnectorPtr = std::shared_ptr<Connector>;
class SSLContext;
/**
 * @brief This class represents a TCP client.
 *
 */
class EASY_EXPORT TcpClient : NonCopyable
{
  public:
    /**
     * @brief Construct a new TCP client instance.
     *
     * @param loop The event loop in which the client runs.
     * @param serverAddr The address of the server.
     * @param nameArg The name of the client.
     */
    TcpClient(EventLoop *loop,
              const InetAddress &serverAddr,
              const std::string &nameArg);
    ~TcpClient();

    /**
     * @brief Connect to the server.
     *
     */
    void connect();

    /**
     * @brief Disconnect from the server.
     *
     */
    void disconnect();

    /**
     * @brief Stop connecting to the server.
     *
     */
    void stop();

    // @brief Get the TCP connection to the server.
    std::shared_ptr<TcpConnection> connection() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return connection_;
    }

    /**
     * @brief Get the event loop.
     *
     * @return EventLoop*
     */
    EventLoop *getLoop() const
    {
        return loop_;
    }

    /**
     * @brief Check whether the client re-connect to the server.
     *
     * @return true
     * @return false
     */
    bool retry() const
    {
        return retry_;
    }

    /**
     * @brief Enable retrying.
     *
     */
    void enableRetry()
    {
        retry_ = true;
    }

    /**
     * @brief Get the name of the client.
     *
     * @return const std::string&
     */
    const std::string &name() const
    {
        return name_;
    }

    /**
     * @brief Set the connection callback.
     *
     * @param cb The callback is called when the connection to the server is
     * established or closed.
     */
    void setConnectionCallback(std::function<void(std::shared_ptr<TcpConnection> const&)> const& cb)
    {
        connectionCallback_ = cb;
    }
    void setConnectionCallback(std::function<void(std::shared_ptr<TcpConnection> const&)> && cb)
    {
        connectionCallback_ = std::move(cb);
    }

    // @note The callback is called when an error occurs during connecting to the server.
    void setConnectionErrorCallback(std::function<void()> const& cb)
    {
        connectionErrorCallback_ = cb;
    }

    // @param cb The callback is called when some data is received from the server.
    void setMessageCallback(std::function<void(std::shared_ptr<TcpConnection> const&, MsgBuffer*)> const& cb) { messageCallback_ = cb; }
    void setMessageCallback(std::function<void(std::shared_ptr<TcpConnection> const&, MsgBuffer*)> && cb) { messageCallback_ = std::move(cb); }
    /// Set write complete callback.
    /// Not thread safe.

    /**
     * @brief Set the write complete callback.
     *
     * @param cb The callback is called when data to send is written to the
     * socket.
     */
    void setWriteCompleteCallback(std::function<void(std::shared_ptr<TcpConnection> const&)> const& cb)
    {
        writeCompleteCallback_ = cb;
    }
    void setWriteCompleteCallback(std::function<void(std::shared_ptr<TcpConnection> const&)> &&cb)
    {
        writeCompleteCallback_ = std::move(cb);
    }

    /**
     * @brief Set the callback for errors of SSL
     * @param cb The callback is called when an SSL error occurs.
     */
    void setSSLErrorCallback(const SSLErrorCallback &cb)
    {
        sslErrorCallback_ = cb;
    }
    void setSSLErrorCallback(SSLErrorCallback &&cb)
    {
        sslErrorCallback_ = std::move(cb);
    }

    /**
     * @brief Enable SSL encryption.
     * @param useOldTLS If true, the TLS 1.0 and 1.1 are supported by the
     * client.
     * @param hostname The server hostname for SNI. If it is empty, the SNI is
     * not used.
     * @param validateCert If true, we try to validate if the peer's SSL cert
     * is valid.
     * @note It's well known that TLS 1.0 and 1.1 are not considered secure in
     * 2020. And it's a good practice to only use TLS 1.2 and above.
     */
    void enableSSL(bool useOldTLS = false,
                   bool validateCert = true,
                   std::string hostname = "");

  private:
    /// Not thread safe, but in loop
    void newConnection(int sockfd);
    /// Not thread safe, but in loop
    void removeConnection(std::shared_ptr<TcpConnection> const& conn);

    EventLoop *loop_;
    ConnectorPtr connector_;  // avoid revealing Connector
    const std::string name_;
    std::function<void(std::shared_ptr<TcpConnection> const&)> connectionCallback_;
    std::function<void()>                                      connectionErrorCallback_;
    std::function<void(std::shared_ptr<TcpConnection> const&, MsgBuffer*)> messageCallback_;
    std::function<void(std::shared_ptr<TcpConnection> const&)> writeCompleteCallback_;
    SSLErrorCallback sslErrorCallback_;
    std::atomic_bool retry_;    // atomic
    std::atomic_bool connect_;  // atomic
    // always in loop thread
    mutable std::mutex mutex_;
    std::shared_ptr<TcpConnection> connection_;  // @GuardedBy mutex_
    std::shared_ptr<SSLContext> sslCtxPtr_;
    bool validateCert_{false};
    std::string SSLHostName_;
#ifndef _WIN32
    class IgnoreSigPipe
    {
      public:
        IgnoreSigPipe()
        {
            ::signal(SIGPIPE, SIG_IGN);
        }
    };

    static IgnoreSigPipe initObj;
#endif
};

}  // namespace easy
