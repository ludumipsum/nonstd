/** Loggin' Utilities
 *  =================
 *  Utility functions and macros for logging, variadic printf-expansion, and
 *  similar chores.
 *
 *  https://www.youtube.com/watch?v=siwpn14IE7E
 */

#pragma once

#include <sstream>

#include <boost/preprocessor/facilities/overload.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "primitive_types.h"

namespace nonstd {
namespace log {

#define GLOBAL_LOGGER_NAME "N2"

/** Basic Logging
 *  -------------
 *  If you want to log a thing, you almost 100% definitely certainly want to
 *  probably use this one.
 *
 *  Usage;
 *      LOG(info)  << "This is an info message";
 *      LOG(err)   << "Uh oh! An error! " << strerr(errno);
 *      Log(debug) << "This has some complex formatting; "
 *                    "{}: ({}) -- {}"_format(foo, bar, baz);
 */
#define LOG(...) \
    BOOST_PP_OVERLOAD(LOG_IMPL, __VA_ARGS__)(__VA_ARGS__)
#define LOG_IMPL1(LEVEL) \
    LOG_IMPL2(::spdlog::get(GLOBAL_LOGGER_NAME), LEVEL)

#define LOG_IMPL2(LOGGER, LEVEL)                                              \
    ::nonstd::log::stream_logger {                                            \
        LOGGER,                                                               \
        ::spdlog::level::LEVEL, /*[trace, debug, info, warn, err, critical]*/ \
        __FILE__, __LINE__, __FUNCTION__                                      \
    }

/** Logging Implementation
 *  ----------------------
 *  An object that will accept and buffer ostream-style char inputs and send the
 *  buffered data to the given spdlog::logger upon destruction.
 *
 *  Usage;
 *      LOG(info)  << "This is an info message";
 *      LOG(err)   << "Uh oh! An error! " << strerr(errno);
 *      Log(debug) << "This has some complex formatting; "
 *                    "{}: ({}) -- {}"_format(foo, bar, baz);
 */
class stream_logger : public ::std::stringstream {
protected:
    ::std::shared_ptr<spdlog::logger> logger;
    ::spdlog::level::level_enum       level;

public:
    stream_logger(::std::shared_ptr<spdlog::logger> logger,
                  ::spdlog::level::level_enum       level,
                  c_cstr                            __file__,
                  i32 const                         __line__,
                  c_cstr                            __function__)
        : logger  ( logger )
        , level   ( level  )
    {
        #if defined(_MSC_VER)
            static const char pathDelimiter = '\\';
        #else
            static const char pathDelimiter = '/';
        #endif
        c_cstr filename = (c_cstr)strrchr(__file__, pathDelimiter);
        if (!filename) { filename = __file__; } //< reset the target
        else           { filename += 1;       } //< move past the leading '/'

        *this << "{}:{} {} -- "_format(filename, __line__, __function__);
    }
    ~stream_logger() {
        logger->log(level, this->str());
    }
};

} /* namespace log */
} /* namespace nonstd */


