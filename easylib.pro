TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lws2_32

INCLUDEPATH += $$PWD/easy \
               $$PWD/include

SOURCES += \
    easy/net/Channel.cxx \
    easy/net/EventLoop.cxx \
    easy/net/EventLoopThread.cxx \
    easy/net/EventLoopThreadPool.cxx \
    easy/net/InetAddress.cxx \
    easy/net/TcpClient.cxx \
    easy/net/TcpServer.cxx \
    easy/net/inner/Acceptor.cxx \
    easy/net/inner/Connector.cxx \
    easy/net/inner/NormalResolver.cxx \
    easy/net/inner/Poller.cxx \
    easy/net/inner/Socket.cxx \
    easy/net/inner/TcpConnectionImpl.cxx \
    easy/net/inner/Timer.cxx \
    easy/net/inner/TimerQueue.cxx \
    easy/net/inner/poller/EpollPoller.cxx \
    easy/net/inner/poller/KQueue.cxx \
    easy/net/inner/poller/Wepoll.c \
    easy/utils/AsyncFileLogger.cxx \
    easy/utils/ConcurrentTaskQueue.cxx \
    easy/utils/Date.cxx \
    easy/utils/LogStream.cxx \
    easy/utils/Logger.cxx \
    easy/utils/MsgBuffer.cxx \
    easy/utils/SerialTaskQueue.cxx \
    easy/utils/TimingWheel.cxx \
    easy/utils/WindowsSupport.cxx \
    main.cxx

HEADERS += \
    easy/Config.hpp \
    easy/net/Channel.h \
    easy/net/EventLoop.h \
    easy/net/EventLoopThread.h \
    easy/net/EventLoopThreadPool.h \
    easy/net/InetAddress.h \
    easy/net/Resolver.h \
    easy/net/TcpClient.h \
    easy/net/TcpConnection.h \
    easy/net/TcpServer.h \
    easy/net/callbacks.h \
    easy/net/inner/Acceptor.h \
    easy/net/inner/AresResolver.h \
    easy/net/inner/Connector.h \
    easy/net/inner/NormalResolver.h \
    easy/net/inner/Poller.h \
    easy/net/inner/Socket.h \
    easy/net/inner/TcpConnectionImpl.h \
    easy/net/inner/Timer.h \
    easy/net/inner/TimerQueue.h \
    easy/net/inner/poller/EpollPoller.h \
    easy/net/inner/poller/KQueue.h \
    easy/net/inner/poller/Wepoll.h \
    easy/utils/AsyncFileLogger.h \
    easy/utils/ConcurrentTaskQueue.h \
    easy/utils/Date.h \
    easy/utils/Funcs.h \
    easy/utils/LockFreeQueue.h \
    easy/utils/LogStream.h \
    easy/utils/Logger.h \
    easy/utils/MsgBuffer.h \
    easy/utils/NonCopyable.h \
    easy/utils/ObjectPool.h \
    easy/utils/SerialTaskQueue.h \
    easy/utils/TaskQueue.h \
    easy/utils/TimingWheel.h \
    easy/utils/WindowsSupport.h



