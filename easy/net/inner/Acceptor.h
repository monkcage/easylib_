
#pragma once

#include <easy/net/EventLoop.h>
#include <easy/utils/NonCopyable.h>
#include "Socket.h"
#include <easy/net/InetAddress.h>
#include "easy/net/Channel.h"
#include <functional>

namespace easy
{
using NewConnectionCallback = std::function<void(int fd, const InetAddress &)>;
class Acceptor : NonCopyable
{
  public:
    Acceptor(EventLoop *loop,
             const InetAddress &addr,
             bool reUseAddr = true,
             bool reUsePort = true);
    ~Acceptor();
    const InetAddress &addr() const
    {
        return addr_;
    }
    void setNewConnectionCallback(const NewConnectionCallback &cb)
    {
        newConnectionCallback_ = cb;
    };
    void listen();

  protected:
#ifndef _WIN32
    int idleFd_;
#endif
    Socket sock_;
    InetAddress addr_;
    EventLoop *loop_;
    NewConnectionCallback newConnectionCallback_;
    Channel acceptChannel_;
    void readCallback();
};
}  // namespace easy
