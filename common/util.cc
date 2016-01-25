#include "util.h"
#include <cstring>

/*## Logging Implementations
*/
#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>

#if defined(_MSC_VER)
    static const char path_delimiter = '\\';
#else
    static const char path_delimiter = '/';
#endif

i32 _logMessage(char const* message,
                char const* file,
                i32 const line,
                char const* function) {
    char* filename = (char*)strrchr(file, path_delimiter);
    if (filename==NULL) {
        return printf("%s:%d %s -- %s\n",
                      file,
                      line,
                      function,
                      message);
    }
    return printf("%s:%d %s -- %s\n",
                  filename+1,
                  line,
                  function,
                  message);
}

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


/*## Configuration Variable Implementations
*/

// Static storage for cvar lists
static CVar_f *head_f = nullptr,
              *tail_f = nullptr;
static CVar_i *head_i = nullptr,
              *tail_i = nullptr;
static CVar_b *head_b = nullptr,
              *tail_b = nullptr;
static CVar_s *head_s = nullptr,
              *tail_s = nullptr;

// list management implementations
template<>
void CVar_f::addToList(void) {
  if(head_f) {
    tail_f->m_next = this;
    tail_f = this;
  } else {
    head_f = this;
    tail_f = this;
  }
}
template<>
void CVar_i::addToList(void) {
  if(head_i) {
    tail_i->m_next = this;
    tail_i = this;
  } else {
    head_i = this;
    tail_i = this;
  }
}
template<>
void CVar_b::addToList(void) {
  if(head_b) {
    tail_b->m_next = this;
    tail_b = this;
  } else {
    head_b = this;
    tail_b = this;
  }
}
void CVar_s::addToList(void) {
  if(head_s) {
    tail_s->m_next = this;
    tail_s = this;
  } else {
    head_s = this;
    tail_s = this;
  }
}

// cvar search (linear scan)
template<>
CVar_f* CVar_f::find(char const* name) {
  CVar_f *cur = head_f;
  while (cur != nullptr && strcmp(name, cur->m_name)) {
    if (0 == strcmp(name, cur->m_name)) break;
    if (cur == tail_f) return nullptr;
    cur = cur->m_next;
  }
  return cur;
}
template<>
CVar_i* CVar_i::find(char const* name) {
  CVar_i* cur = head_i;
  while (cur != nullptr) {
    if (0 == strcmp(name, cur->m_name)) break;
    if (cur == tail_i) return nullptr;
    cur = cur->m_next;
  }
  return cur;
}
template<>
CVar_b* CVar_b::find(char const* name) {
  CVar_b* cur = head_b;
  while (cur != nullptr && strcmp(name, cur->m_name)) {
    if (0 == strcmp(name, cur->m_name)) break;
    if (cur == tail_b) return nullptr;
    cur = cur->m_next;
  }
  return cur;
}
CVar_s* CVar_s::find(char const* name) {
  CVar_s* cur = head_s;
  while (cur != nullptr && strcmp(name, cur->m_name)) {
    if (0 == strcmp(name, cur->m_name)) break;
    if (cur == tail_s) return nullptr;
    cur = cur->m_next;
  }
  return cur;
}


/* Deliberate Crash Handler Implementation
   =======================================
*/

#include <setjmp.h>
#include <string.h>
#include <inttypes.h>

static CVar_i g_crashstring_max_size("Mem/CrashStringLength",
                                     "Maximum bytes of memory to use for crash "
                                     "message processing", 4096);

void _CRASH(int error_number,
            char const* reason,
            char const* file,
            uint64_t line,
            char const* funcsig) {
  // Some tests deliberately trigger crashes; for those, we jump back
  // to the saved environment and continue.
  #if defined(N2_TEST)
  UNUSED(reason); UNUSED(file); UNUSED(line); UNUSED(funcsig);
    // Expected failure, jump to the test case and continue
    if (g_crash_expected) {
        // TODO: once we have log filtering, re-enable this
        // LOG(LOG_LEVEL_TRACE, "Trapped testing crash: (%d) %s",
        //                      error_number, strerror(error_number));
        longjmp(g_crash_jmp_target, error_number);
    }
  #else
    // Interpolate crash report with error information
    char* errstring = (char*)malloc((const size_t)CV(g_crashstring_max_size));
    snprintf(errstring, (size_t) CV(g_crashstring_max_size),
                        "Crashing in %s (%s:%" PRIu64 ")!\n"
                        "ERRNO: %d (%s)\n" "Reason: %s",
                        funcsig, file, line,
                        error_number, strerror(error_number),
                        reason);
    LOG("Fatal Error: %s", errstring);
    BREAKPOINT;
    exit(error_number);
  #endif
}
