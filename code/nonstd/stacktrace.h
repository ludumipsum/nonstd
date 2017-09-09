/** Signal Handlers
 *  ===============
 *  A utility for dumping stacktraces in signal handler callbacks, and a
 *  convenience macro for registering it as a callback for the set of signals
 *  we consider crashes.
 */

#pragma once

// This whole thing only works on non-msvc compilers
#if !defined(_MSC_VER)

#include "batteries_included.h"
#include "primitive_types.h"

#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const u32 g_trace_skip = 1;

#if defined(__APPLE__)
#define SIGTABLE sys_signame
#else
#define SIGTABLE sys_siglist
#endif

inline void stacktrace_callback(int signum) {
    void *stack[128];
    u32 max_frame_count = sizeof(stack) / sizeof(stack[0]);

    // get the list of frame pointers on the stack
    u32 frame_count = backtrace(stack, max_frame_count);

    // symbolize all the frames we got in the trace
    cstr* symbols = backtrace_symbols(stack, frame_count);

    // Flush output buffers before producing a trace table
    fflush(stdout);
    fflush(stderr);

    // Print an error trap banner
    cstr signame = strdup(SIGTABLE[signum]);
    u32 namelen = strlen(signame);
    for (u32 i = 0; i < namelen; ++i) { signame[i] = toupper(signame[i]); }
    fprintf(stderr, "\n***** CAUGHT SIG%s (%d) *****\n\n",
                    signame, signum);
    free(signame);

    // Print a trace table header
    fprintf(stderr, "FRAME            ADDRESS   SYMBOL + OFFSET\n");
    fprintf(stderr, "-----   ----------------   ---------------\n");

    // Iterate over each frame and print the data we can get out of it
    c_cstr previous_fname = nullptr;
    for (u32 i = g_trace_skip; i < frame_count; ++i) {
        // Figure out how to display the symbol's name
        Dl_info info;
        if (dladdr(stack[i], &info) && info.dli_sname) {
            if (info.dli_fname != previous_fname) {
                previous_fname = info.dli_fname;
                auto print_addr = strrchr(previous_fname, '/') + 1;
                fprintf(stderr, "%s\n", print_addr);
            }

            cstr demangled = nullptr;
            int demangle_status = -2;
            // Detect mangled C++ names
            if (info.dli_sname[0] == '_') {
                demangled = abi::__cxa_demangle(
                    info.dli_sname,
                    nullptr, /* output_buffer */
                    0,       /* length */
                    &demangle_status);
            }

            // Fish out the most precise name we have for the symbol
            c_cstr demangle_detail = nullptr;
            if (demangle_status == 0) {
                demangle_detail = demangled;
            } else {
                if (info.dli_sname == 0) {
                    demangle_detail = symbols[i];
                } else {
                    demangle_detail = info.dli_sname;
                }
            }

            // Calculate the offset point of the trace into the function
            auto trace_offset = (char *)stack[i] - (char *)info.dli_saddr;

            // Dump the result to stderr
            fprintf(stderr, "%5d %*p   %s + %zd\n",
                            i,                          /* frame number */
                            u32(2 + sizeof(void*) * 2), /* padding */
                            stack[i],                   /* frame address */
                            demangle_detail,            /* demangled name */
                            trace_offset                /* trace offset */
            );

            free(demangled);
            demangled = nullptr;
        }
    }

    free(symbols);

    fprintf(stderr, "\n");

    exit(signum);
}

#define REGISTER_STACK_HANDLERS()             \
        signal(SIGSEGV, stacktrace_callback); \
        signal(SIGINT, stacktrace_callback);  \
        signal(SIGHUP, stacktrace_callback)

#else

#define REGISTER_STACK_HANDLERS()

#endif
