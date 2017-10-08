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
 *  TODO: For some reason `auto logger = LOG(info);` fails to compile; it's
 *        attempting to invoke the copy ctor (rather than the move ctor), and
 *        std::stringstream (which is the parent class of stream_logger)
 *        explicitly deletes its copy ctor. It would be nice to remove the SLOG
 *        macros and just use the above, but... :shrugs:
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
using levels_t = spdlog::level::level_enum;
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
 *  We're using a stateful logging system, so set up that state.
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
#if !BOOST_PP_VARIADICS_MSVC
#  define LOG(...) \
      BOOST_PP_OVERLOAD(LOG_IMPL, __VA_ARGS__)(__VA_ARGS__)
#else // MSVC doesn't comply with C99 macro parsing. Gotta work around that.
#  define LOG(...) \
      BOOST_PP_CAT(BOOST_PP_OVERLOAD(LOG_IMPL,__VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY())
#endif
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
 *      {SCOPED_LOG(logger, info)
 *          logger << "Some text\n";
 *          for (auto && i : Range(10)) {
 *              logger.align() << i << " more text\n";
 *          }
 *      }
 */
#define SLOG(...) SCOPED_LOG(__VA_ARGS__)
#if !BOOST_PP_VARIADICS_MSVC
#  define SCOPED_LOG(...) \
      BOOST_PP_OVERLOAD(SCOPED_LOG_IMPL, __VA_ARGS__)(__VA_ARGS__)
#else // MSVC doesn't comply with C99 macro parsing. Gotta work around that.
#  define SCOPED_LOG(...) \
      BOOST_PP_CAT(BOOST_PP_OVERLOAD(SCOPED_LOG_IMPL,__VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY())
#endif
#define SCOPED_LOG(...) \
    BOOST_PP_OVERLOAD(SCOPED_LOG_IMPL, __VA_ARGS__)(__VA_ARGS__)
#define SCOPED_LOG_IMPL2(NAME, LEVEL) \
    SCOPED_LOG_IMPL3(NAME, ::spdlog::get(::nonstd::log::globalLoggerName), LEVEL)
/* We play with the formatting here to make error messages look better */
#define SCOPED_LOG_IMPL3(NAME, LOGGER, LEVEL) \
  ::nonstd::log::stream_logger NAME {         \
    LOGGER,                                   \
    ::nonstd::log::levels::LEVEL, /* ## Should be [trace, debug, info, warn, error, or critical] */\
    __FILE__, __LINE__, __FUNCTION__ };

/** Aligned Log Line
 *  ----------------
 *  See the `aligned_newline_t` below
 */
#define LOG_LINE ::nonstd::log::aligned_newline


/** Aligned Log Newline Type
 *  ------------------------
 *  Tag Type to explicitly inject an aligned newline into a log stream.
 *  Aliased from the LOG_LINE #define.
 *
 *  Usage;
 *      LOG(info) << "One line..."    << nonstd::log::aligned_newline
 *                << "Another line."  << nonstd::log::aligned_newline
 *                << "So many lines!" << nonstd::log::aligned_newline
 */
struct aligned_newline_t {
    /* Explicitly delete the default constructor, and define an explicit
       constexpr constructor that will be hard to accidentally invoke.
       Let's use the `nonstd::in_place_t` tag for that. */
    aligned_newline_t() = delete;
    explicit constexpr aligned_newline_t(nonstd::in_place_t) { }
};
constexpr static aligned_newline_t aligned_newline { nonstd::in_place };


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
 *
 *  If a single log message needs to be built up over over multiple calls, the
 *  SCOPED_LOG macro can be used to construct and retain a logger object.
 *
 *  Usage;
 *      {SCOPED_LOG(logger, info)
 *          logger << "Some text";
 *          for (auto && i : Range(10)) {
 *              logger.line() << i << " more text";
 *          }
 *      }
 */
class stream_logger : public stringstream {
protected:
    shared_ptr<spdlog::logger>  logger;
    ::spdlog::level::level_enum level;
    bool                        should_log;
    size_t                      padding;

public:
    stream_logger(shared_ptr<spdlog::logger> logger,
                  ::nonstd::log::levels_t    level,
                  c_cstr                     __file__,
                  i32 const                  __line__,
                  c_cstr                     __function__)
        : logger     ( logger )
        , level      ( level  )
        , should_log ( true   )
        , padding    ( 0      ) //< Will be calculated in the ctor body
    {
        // If possible, remove the /path/to the file name.
        c_cstr filename = (c_cstr)strrchr(__file__, preferred_separator);
        if (!filename) { filename = __file__; } //< reset the target
        else           { filename += 1;       } //< move past the leading '/'

        *this << "{}:{} {} -- "_format(filename, __line__, __function__);

        // Capture the amount of padding needed to align "\n"s with the initial
        // spdlog + file/line/fn preamble
        padding = 30 //< length of spdlog preamble, discounting logger title
                + this->logger->name().length() //< length of the logger title
                + this->length()                //< length of the above insert
                -  2;                           //< number of special chars
    }
    ~stream_logger() {
        if (should_log) { logger->log(level, this->str()); }
    }

    /** Conditional Logging
     *  -------------------
     *  LOG(critical).when(false) << "This will never log!";
     *  LOG(critical).unless(keep_quiet) << "Should I make noise?";
     */
    inline stream_logger & when(bool cond) noexcept {
        should_log = cond;
        return *this;
    }
    inline stream_logger & unless(bool cond) noexcept {
        should_log = !cond;
        return *this;
    }

    /** Insertion Operator Overloads
     *  ----------------------------
     *  Overload **everything** s.t. chains of `<<` operators correctly return
     *  references to stream_logger, rather than stringstream.
     *  I cannot imagine this is cheap to compile....
     */
    template<typename T>
    stream_logger & operator<< (T const & v) {
        (*(std::ostringstream*)this) << v;
        return *this;
    }


    /** Line Injection
     *  --------------
     *  The magic that injects padded lines.
     */
    inline stream_logger & line() {
        *this << "\n" << std::string(padding, ' ') << ".. ";
        return *this;
    }
    inline stream_logger & operator<< (aligned_newline_t /*unused*/) {
        *this << "\n" << std::string(padding, ' ') << ".. ";
        return *this;
    }

    /** Length Helper
     *  -------------
     *  Get the current length of the underlying buffer **without copies**.
     */
    inline u32 length() {
        // Capture the current write head offset
        stream_logger::pos_type current_position = this->tellg();
        // Move to the end, get the current length
        this->seekg(0, stream_logger::end);
        u32 ret = this->tellg();
        // Reset the write head
        this->seekg(current_position, stream_logger::beg);

        return ret;
    }
};

} /* namespace log */
} /* namespace nonstd */
