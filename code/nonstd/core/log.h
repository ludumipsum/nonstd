/** Loggin' Utilities
 *  =================
 *  Utility functions, classes, and macros for logging.
 *  https://www.youtube.com/watch?v=siwpn14IE7E
 *
 *  TODO: Always initializing into async mode makes test output kinda weird. I
 *        don't know how to make this configurable, but we should make
 *        that configurable.
 *  TODO: The basic, LOG/1 macro relies on `spdlog::get(globalLoggerName)` to
 *        find itself a logger instance with which to log. This locks a mutex,
 *        and performs a std::unsorted_map::find. Seems less than ideal for our
 *        basic use case.
 *        Figure out if there's something we can do to easily store the
 *        std::shared_ptr that spdlog::get returns, and use that in our macros.
 */

#pragma once

#include <sstream>

#include <boost/preprocessor/facilities/overload.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "primitive_types.h"

namespace nonstd {
namespace log {

using ::std::shared_ptr;
using ::std::stringstream;
using ::nonstd::preferred_separator;

/* Name of the should-always-be-available logger instance. */
constexpr c_cstr globalLoggerName = "N2";

/** Log Levels
 *  ----------
 *  Map spdlog log levels into a namespace that we control for easier writing,
 *  reading, and debugging.
 */
using levels_t = typename spdlog::level::level_enum;
namespace levels {
    constexpr levels_t trace    = spdlog::level::level_enum::trace;
    constexpr levels_t debug    = spdlog::level::level_enum::debug;
    constexpr levels_t info     = spdlog::level::level_enum::info;
    constexpr levels_t warning  = spdlog::level::level_enum::warn;
    constexpr levels_t warn     = spdlog::level::level_enum::warn;
    constexpr levels_t error    = spdlog::level::level_enum::err;
    constexpr levels_t err      = spdlog::level::level_enum::err;
    constexpr levels_t critical = spdlog::level::level_enum::critical;
    constexpr levels_t crit     = spdlog::level::level_enum::critical;
}

/** Initialization Logic
 *  --------------------
 *  We're using a stateful logging system, so set it up state.
 */
inline void init() {
    // Configure spdlog to log asynchronously.
    // From https://github.com/gabime/spdlog/wiki/6.-Asynchronous-logging
    //
    // > Memory consumption (important)
    // >
    // > Each logger pre allocates on the heap a buffer whose size is
    // > `queue_size` x `sizeof(slot)`.
    // >
    // > Each slot (in 64 bits) is 104 bytes, so for example
    // > `set_async_mode(8192)` would allocate on the heap 8192*104=832KB for
    // > each async logger.
    spdlog::set_async_mode(1024);

    // Configure the default spdlog pattern.
    // 1. HH:MM:SS.Microseconds
    // 2. Thread ID
    // 3. Logger Name
    // 4. Log Text        1.      2.   3.  4.
    spdlog::set_pattern("[%T.%f] [%t] [%n] %v");

    // Initialize the global logger.
    spdlog::stdout_color_mt(globalLoggerName);
}


/** Basic Logging
 *  -------------
 *  If you want to log a thing, you almost 100% definitely certainly want to
 *  probably use this one.
 *
 *  Usage;
 *      LOG(info)  << "This is an info message";
 *      LOG(error) << "Uh oh! An error! " << strerr(errno);
 *      Log(debug) << "This has some complex formatting; "
 *                    "{}:{} -- ({})"_format(foo, bar, baz);
 */
#define LOG(...) \
    BOOST_PP_OVERLOAD(LOG_IMPL, __VA_ARGS__)(__VA_ARGS__)
#define LOG_IMPL1(LEVEL) \
    LOG_IMPL2(::spdlog::get(::nonstd::log::globalLoggerName), LEVEL)
/* We play with the formatting here to make error messages look better */
#define LOG_IMPL2(LOGGER, LEVEL)     \
  ::nonstd::log::stream_logger {     \
    LOGGER,                          \
    ::nonstd::log::levels::LEVEL, /* ## Should be [trace, debug, info, warn, error, or critical] */\
    __FILE__, __LINE__, __FUNCTION__ }

/** Persisted Logging
 *  -----------------
 *  If you want to aggregate multiple log lines across multiple calls into a
 *  single log entry, you can definitely choose to use this instead of something
 *  that's not this.
 *
 *  Usage;
 *      { SCOPE_LOG(logger, info)
 *          logger << "Some text\n";
 *          for (auto && i : Range(10)) {
 *              logger.align() << i << " more text\n";
 *          }
 *      }
 */
#define SCOPE_LOG(...) \
    BOOST_PP_OVERLOAD(SCOPE_LOG_IMPL, __VA_ARGS__)(__VA_ARGS__)
#define SCOPE_LOG_IMPL2(NAME, LEVEL) \
    SCOPE_LOG_IMPL3(NAME, ::spdlog::get(::nonstd::log::globalLoggerName), LEVEL)
/* We play with the formatting here to make error messages look better */
#define SCOPE_LOG_IMPL3(NAME, LOGGER, LEVEL) \
  ::nonstd::log::scope_logger NAME {         \
    LOGGER,                                  \
    ::nonstd::log::levels::LEVEL, /* ## Should be [trace, debug, info, warn, error, or critical] */\
    __FILE__, __LINE__, __FUNCTION__ };


/** Basic Logging Object
 *  --------------------
 *  An object that will accept and buffer ostream-style char inputs and send the
 *  buffered data to the given spdlog::logger upon destruction.
 *
 *  Usage;
 *      LOG(info)  << "This is an info message";
 *      LOG(error) << "Uh oh! An error! " << strerr(errno);
 *      Log(debug) << "This has some complex formatting; "
 *                    "{}:{} -- ({})"_format(foo, bar, baz);
 */
class stream_logger : public stringstream {
protected:
    shared_ptr<spdlog::logger>  logger;
    ::spdlog::level::level_enum level;
    bool                        should_log;

public:
    stream_logger(shared_ptr<spdlog::logger> logger,
                  ::nonstd::log::levels_t    level,
                  c_cstr                     __file__,
                  i32 const                  __line__,
                  c_cstr                     __function__)
        : logger     ( logger )
        , level      ( level  )
        , should_log ( true   )
    {
        c_cstr filename = (c_cstr)strrchr(__file__, preferred_separator);
        if (!filename) { filename = __file__; } //< reset the target
        else           { filename += 1;       } //< move past the leading '/'

        *this << "{}:{} {} -- "_format(filename, __line__, __function__);
    }
    ~stream_logger() {
        if (should_log) { logger->log(level, this->str()); }
    }

    inline stringstream & when(bool cond) noexcept {
        should_log = cond;
        return *this;
    }
    inline stringstream & unless(bool cond) noexcept {
        should_log = !cond;
        return *this;
    }
};


/** Persisted Logging Object
 *  ------------------------
 *  Specialization on stream_logger that captures some additional metadata from
 *  the stream_logger initialization, and exposes a `.align()` method that
 *  allows instances of this class to insert whitespace equal to the length of
 *  the spdlog and log macro preambles.
 *
 *  Usage;
 *      { SCOPE_LOG(logger, info)
 *          logger << "Some text\n";
 *          for (auto && i : Range(10)) {
 *              logger.align() << i << " more text\n";
 *          }
 *      }
 */
class scope_logger : public stream_logger {
protected:
    size_t padding;

public:
    scope_logger(shared_ptr<spdlog::logger>  logger,
                 ::spdlog::level::level_enum level,
                 c_cstr                      __file__,
                 i32 const                   __line__,
                 c_cstr                      __function__)
        : stream_logger(logger, level, __file__, __line__, __function__)
    {
        padding = 30 //< length of spdlog preamble, discounting logger title
                + this->logger->name().length() //< length of the logger title
                + this->str().length()          //< length of the macro preamble
                -  2;                           //< number of special chars
    }

    inline stringstream & align() {
        *this << std::string(padding, ' ') << ".. ";
        return *this;
    }
};

} /* namespace log */
} /* namespace nonstd */
