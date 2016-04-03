/* Loggin Utilities
   ================

   Utility functions and macros for logging, variadic printf-expansion, and
   similar chores.
*/


/* Stringifcation Functions
   ========================
   for easy printf-ing.
*/
inline char const* const bool2string(bool b) {
  return b ? "true" : "false";
};


/* Variadic Message Format Expander
   ================================
  _variadicExpand allows the `MESSAGE` parameter that's passed into `LOG` take
  the form of a format string.
  Modified from Erik Aronesty's answer of this SO question:
  http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
  but also: http://stackoverflow.com/questions/7825648/vscprintf-equivalent-on-android/
*/
std::string _variadicExpand(char const* message, ...)
{
    va_list format_args;

    va_start(format_args, message);
    u32 size = vsnprintf(NULL, 0, message, format_args);
    va_end(format_args);

    std::string expanded;
    expanded.resize(size + 1);

    va_start(format_args, message);
    vsnprintf(const_cast<char*>(expanded.c_str()), size + 1, message, format_args);
    va_end(format_args);

    return expanded;
}

/* Logging Implementation
   ======================
*/
i32 _logMessage(char const* message,
                char const* file,
                i32  const  line,
                char const* function)
{
    char* filename = (char*)strrchr(file, path_delimiter);
    if (filename==NULL) {
        return printf("%s:%d %s -- %s\n",
                      file, line,
                      function, message);
    }
    return printf("%s:%d %s -- %s\n",
                  filename+1, line,
                  function, message);
}

/* Logging Macro Shorthand
   =======================
*/
#define LOG(MESSAGE, ...) \
    _logMessage(::_variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
                __FILE__, \
                __LINE__, \
                __FUNCTION__)
