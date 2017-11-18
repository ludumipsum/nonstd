#pragma once

#define ENABLE_REQUIRE_LINKAGE_AS(name) \
    int force_link_##name = 0;

#define REQUIRE_LINKAGE_WITH(name) \
    void force_link_function_##name(void) { \
        extern int force_link_##name;       \
        force_link_##name = 1;              \
    }
