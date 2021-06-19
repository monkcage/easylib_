
#include "Poller.h"
#ifdef __linux__
#include "poller/EpollPoller.h"
#elif defined _WIN32
#include "poller/Wepoll.h"
#include "poller/EpollPoller.h"
#else
#include "poller/KQueue.h"
#endif
using namespace easy;
Poller *Poller::newPoller(EventLoop *loop)
{
#if defined __linux__ || defined _WIN32
    return new EpollPoller(loop);
#else
    return new KQueue(loop);
#endif
}
