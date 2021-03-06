

#pragma once

#include <easy/utils/NonCopyable.h>
#include <easy/utils/Date.h>
#include <easy/utils/LogStream.h>
#include <easy/Config.hpp>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>

namespace easy
{
/**
 * @brief This class implements log functions.
 *
 */
class EASY_EXPORT Logger : public NonCopyable
{
  public:
    enum LogLevel
    {
        kTrace = 0,
        kDebug,
        kInfo,
        kWarn,
        kError,
        kFatal,
        kNumberOfLogLevels
    };

    /**
     * @brief Calculate of basename of source files in compile time.
     *
     */
    class SourceFile
    {
      public:
        template <int N>
        inline SourceFile(const char (&arr)[N]) : data_(arr), size_(N - 1)
        {
            // std::cout<<data_<<std::endl;
            const char *slash = strrchr(data_, '/');  // builtin function
            if (slash)
            {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }

        explicit SourceFile(const char *filename) : data_(filename)
        {
            const char *slash = strrchr(filename, '/');
            if (slash)
            {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(strlen(data_));
        }

        const char *data_;
        int size_;
    };
    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, bool isSysErr);
    Logger(SourceFile file, int line, LogLevel level, const char *func);
    ~Logger();
    Logger &setIndex(int index)
    {
        index_ = index;
        return *this;
    }
    LogStream &stream();

    /**
     * @brief Set the output function.
     *
     * @param outputFunc The function to output a log message.
     * @param flushFunc The function to flush.
     * @note Logs are output to the standard output by default.
     */
    static void setOutputFunction(
        std::function<void(const char *msg, const uint64_t len)> outputFunc,
        std::function<void()> flushFunc,
        int index = -1)
    {
        if (index < 0)
        {
            outputFunc_() = outputFunc;
            flushFunc_() = flushFunc;
        }
        else
        {
            outputFunc_(index) = outputFunc;
            flushFunc_(index) = flushFunc;
        }
    }

    /**
     * @brief Set the log level. Logs below the level are not printed.
     *
     * @param level
     */
    static void setLogLevel(LogLevel level)
    {
        logLevel_() = level;
    }

    /**
     * @brief Get the current log level.
     *
     * @return LogLevel
     */
    static LogLevel logLevel()
    {
        return logLevel_();
    }

  protected:
    static void defaultOutputFunction(const char *msg, const uint64_t len)
    {
        fwrite(msg, 1, len, stdout);
    }
    static void defaultFlushFunction()
    {
        fflush(stdout);
    }
    void formatTime();
    static LogLevel &logLevel_()
    {
#ifdef RELEASE
        static LogLevel logLevel = LogLevel::kInfo;
#else
        static LogLevel logLevel = LogLevel::kDebug;
#endif
        return logLevel;
    }
    static std::function<void(const char *msg, const uint64_t len)>
        &outputFunc_()
    {
        static std::function<void(const char *msg, const uint64_t len)>
            outputFunc = Logger::defaultOutputFunction;
        return outputFunc;
    }
    static std::function<void()> &flushFunc_()
    {
        static std::function<void()> flushFunc = Logger::defaultFlushFunction;
        return flushFunc;
    }
    static std::function<void(const char *msg, const uint64_t len)>
        &outputFunc_(size_t index)
    {
        static std::vector<
            std::function<void(const char *msg, const uint64_t len)>>
            outputFuncs;
        if (index < outputFuncs.size())
        {
            return outputFuncs[index];
        }
        while (index >= outputFuncs.size())
        {
            outputFuncs.emplace_back(outputFunc_());
        }
        return outputFuncs[index];
    }
    static std::function<void()> &flushFunc_(size_t index)
    {
        static std::vector<std::function<void()>> flushFuncs;
        if (index < flushFuncs.size())
        {
            return flushFuncs[index];
        }
        while (index >= flushFuncs.size())
        {
            flushFuncs.emplace_back(flushFunc_());
        }
        return flushFuncs[index];
    }
    friend class RawLogger;
    LogStream logStream_;
    Date date_{Date::now()};
    SourceFile sourceFile_;
    int fileLine_;
    LogLevel level_;
    int index_{-1};
};
class EASY_EXPORT RawLogger : public NonCopyable
{
  public:
    ~RawLogger();
    RawLogger &setIndex(int index)
    {
        index_ = index;
        return *this;
    }
    LogStream &stream()
    {
        return logStream_;
    }

  private:
    LogStream logStream_;
    int index_{-1};
};
#ifdef NDEBUG
#define LOG_TRACE                                                          \
    if (0)                                                                 \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kTrace, __func__) \
        .stream()
#else
#define LOG_TRACE                                                          \
    if (easy::Logger::logLevel() <= easy::Logger::kTrace)            \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kTrace, __func__) \
        .stream()
#define LOG_TRACE_TO(index)                                                \
    if (easy::Logger::logLevel() <= easy::Logger::kTrace)            \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kTrace, __func__) \
        .setIndex(index)                                                   \
        .stream()

#endif

#define LOG_DEBUG                                                          \
    if (easy::Logger::logLevel() <= easy::Logger::kDebug)            \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kDebug, __func__) \
        .stream()
#define LOG_DEBUG_TO(index)                                                \
    if (easy::Logger::logLevel() <= easy::Logger::kDebug)            \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kDebug, __func__) \
        .setIndex(index)                                                   \
        .stream()
#define LOG_INFO                                               \
    if (easy::Logger::logLevel() <= easy::Logger::kInfo) \
    easy::Logger(__FILE__, __LINE__).stream()
#define LOG_INFO_TO(index)                                     \
    if (easy::Logger::logLevel() <= easy::Logger::kInfo) \
    easy::Logger(__FILE__, __LINE__).setIndex(index).stream()
#define LOG_WARN \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kWarn).stream()
#define LOG_WARN_TO(index)                                      \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kWarn) \
        .setIndex(index)                                        \
        .stream()
#define LOG_ERROR \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kError).stream()
#define LOG_ERROR_TO(index)                                      \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kError) \
        .setIndex(index)                                         \
        .stream()
#define LOG_FATAL \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kFatal).stream()
#define LOG_FATAL_TO(index)                                      \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kFatal) \
        .setIndex(index)                                         \
        .stream()
#define LOG_SYSERR easy::Logger(__FILE__, __LINE__, true).stream()
#define LOG_SYSERR_TO(index) \
    easy::Logger(__FILE__, __LINE__, true).setIndex(index).stream()

#define LOG_RAW easy::RawLogger().stream()
#define LOG_RAW_TO(index) easy::RawLogger().setIndex(index).stream()

#define LOG_TRACE_IF(cond)                                                  \
    if ((easy::Logger::logLevel() <= easy::Logger::kTrace) && (cond)) \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kTrace, __func__)  \
        .stream()
#define LOG_DEBUG_IF(cond)                                                \
    if ((Tensor::Logger::logLevel() <= Tensor::Logger::kDebug) && (cond)) \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__)  \
        .stream()
#define LOG_INFO_IF(cond)                                                \
    if ((Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) && (cond)) \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN_IF(cond) \
    if (cond)             \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define LOG_ERROR_IF(cond) \
    if (cond)              \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define LOG_FATAL_IF(cond) \
    if (cond)              \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#ifdef NDEBUG
#define DLOG_TRACE                                                         \
    if (0)                                                                 \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kTrace, __func__) \
        .stream()
#define DLOG_DEBUG                                                       \
    if (0)                                                               \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
        .stream()
#define DLOG_INFO \
    if (0)        \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN \
    if (0)        \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR \
    if (0)         \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL \
    if (0)         \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#define DLOG_TRACE_IF(cond)                                                \
    if (0)                                                                 \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kTrace, __func__) \
        .stream()
#define DLOG_DEBUG_IF(cond)                                              \
    if (0)                                                               \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
        .stream()
#define DLOG_INFO_IF(cond) \
    if (0)                 \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN_IF(cond) \
    if (0)                 \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR_IF(cond) \
    if (0)                  \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL_IF(cond) \
    if (0)                  \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()
#else
#define DLOG_TRACE                                                         \
    if (easy::Logger::logLevel() <= easy::Logger::kTrace)            \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kTrace, __func__) \
        .stream()
#define DLOG_DEBUG                                                       \
    if (Tensor::Logger::logLevel() <= Tensor::Logger::kDebug)            \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
        .stream()
#define DLOG_INFO                                            \
    if (Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#define DLOG_TRACE_IF(cond)                                                 \
    if ((easy::Logger::logLevel() <= easy::Logger::kTrace) && (cond)) \
    easy::Logger(__FILE__, __LINE__, easy::Logger::kTrace, __func__)  \
        .stream()
#define DLOG_DEBUG_IF(cond)                                               \
    if ((Tensor::Logger::logLevel() <= Tensor::Logger::kDebug) && (cond)) \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__)  \
        .stream()
#define DLOG_INFO_IF(cond)                                               \
    if ((Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) && (cond)) \
    Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN_IF(cond) \
    if (cond)              \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR_IF(cond) \
    if (cond)               \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL_IF(cond) \
    if (cond)               \
    Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()
#endif

const char *strerror_tl(int savedErrno);
}  // namespace easy
