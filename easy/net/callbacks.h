

#pragma once

#include <functional>
#include <memory>
namespace easy
{



enum class SSLError
{
    kSSLHandshakeError,
    kSSLInvalidCertificate
};


using TimerCallback = std::function<void()>;

// the data has been read to (buf, len)
class TcpConnection;
class MsgBuffer;
//using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
// tcp server and connection callback
// using RecvMessageCallback =
//     std::function<void(const TcpConnectionPtr &, MsgBuffer *)>;
// using ConnectionErrorCallback = std::function<void()>;
// using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
// using CloseCallback = std::function<void(const TcpConnectionPtr &)>;
// using WriteCompleteCallback = std::function<void(const TcpConnectionPtr &)>;
// 


using SSLErrorCallback = std::function<void(SSLError)>;

}  // namespace easy
