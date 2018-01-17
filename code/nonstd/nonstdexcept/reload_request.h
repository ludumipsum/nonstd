/** Exception Type for Requesting Hotloads
 *  ======================================
 */

#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/optional.h>

struct reload_request : public std::exception {
    reload_request()
        : maybe_library(nonstd::none<std::string>())
        , message("Hotload the current library")
        { }
    reload_request(std::string const& library_name)
        : maybe_library(library_name)
        , message("Hotload to {}"_format(maybe_library))
        { }
    virtual ~reload_request() noexcept {}

    virtual c_cstr what() const noexcept {
        return message.c_str();
    }

    nonstd::optional<std::string> maybe_library;
    std::string message;
};
