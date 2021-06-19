#ifndef EASY_WINDOWS_SUPPORT_INC
#define EASY_WINDOWS_SUPPORT_INC

#include <easy/Config.hpp>
#include <stdint.h>

struct iovec
{
    void *iov_base; /* Starting address */
    int iov_len;    /* Number of bytes */
};

EASY_EXPORT int readv(int fd, const struct iovec *vector, int count);



#endif
