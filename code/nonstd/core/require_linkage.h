/** Require Linkage
 *  ===============
 *  The linker is a helpful tool. It help reduce the size of the binaries it
 *  links by culling unused symbols. Sometimes, it feels so good about culling
 *  symbols from a file, that it'll cull the entire file.
 *  Sometimes that's not okay.
 *
 *  To guarantee that the linker does not cull a file of yours, you can place
 *  `ENABLE_REQUIRE_LINKAGE_AS(unique_identifier)` in the file that you want
 *  to protect, and `REQUIRE_LINKAGE_WITH(unique_identifier)` in a file that
 *  links against the protected file.
*/
#pragma once

#define ENABLE_REQUIRE_LINKAGE_AS(name) \
    int force_link_##name = 0;

#define REQUIRE_LINKAGE_WITH(name) \
    void force_link_function_##name(void) { \
        extern int force_link_##name;       \
        force_link_##name = 1;              \
    }
