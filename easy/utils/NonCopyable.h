

#pragma once

#include <easy/Config.hpp>

namespace easy
{
/**
 * @brief This class represents a non-copyable object.
 *
 */
class EASY_EXPORT NonCopyable
{
  protected:
    NonCopyable()
    {
    }
    ~NonCopyable()
    {
    }
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
    // some uncopyable classes maybe support move constructor....
    NonCopyable(NonCopyable &&) noexcept(true) = default;
    NonCopyable &operator=(NonCopyable &&) noexcept(true) = default;
};

}  // namespace easy
