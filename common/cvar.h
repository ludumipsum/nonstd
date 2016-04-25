/* Configuration Variables
   =======================

   DSS/DATA allocated configuration variable list.

   CVars may be queried and set from anywhere, but may only be defined and
   watched from platform code. They are stored in the DSS/DATA segment of the
   platform binary, and are meant for configuration of platform and system
   settings, such as graphics modes, feature flags, and simulation parameters.

   CVars should be defined at a single static global location, and may be used
   from other files by including the module in which they are defined, or by
   runtime lookup with CVar_t::find(). They can also be watched for changes by
   a single std::function().
*/

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "debug.h"

/* Return the value contained in the pointed-to cvar, or nullstr / zero */
#define SCV(some_cvar_p) ((some_cvar_p == NULL) ? some_cvar_p->value() : NULL)
/* Return the value contained in `some_cvar` */
#define CV(some_cvar) ((some_cvar).value())


// TODO: config file generator from defined CVars
// TODO: cvars auto-exposed as command line flags

template<typename VALUE_TYPE> class CVar;

typedef CVar<double>      CVar_f;
typedef CVar<int64_t>     CVar_i;
typedef CVar<bool>        CVar_b;
typedef CVar<c_cstr>      CVar_s;


template<typename VALUE_TYPE>
class CVar {
public:
    CVar(c_cstr name, c_cstr synopsis, VALUE_TYPE value)
        : m_next           ( nullptr  )
        , m_name           ( name     )
        , m_synopsis       ( synopsis )
        , m_value          ( value    )
        , m_min            ( value    )
        , m_max            ( value    )
        , m_watch_callback ( nullptr  )
        , m_storage        ( ""       )
    {
        addToList();
    }
    CVar(c_cstr name,
         c_cstr synopsis,
         VALUE_TYPE value,
         VALUE_TYPE min,
         VALUE_TYPE max)
        : m_next           ( nullptr  )
        , m_name           ( name     )
        , m_synopsis       ( synopsis )
        , m_value          ( value    )
        , m_min            ( min      )
        , m_max            ( max      )
        , m_watch_callback ( nullptr  )
        , m_storage        ( ""       )
    {
        addToList();
    }

    // Transparent assignment as VALUE_TYPE
    using is_str = std::is_same<c_cstr, VALUE_TYPE>;
    TEMPLATE_ENABLE(!is_str::value, VALUE_TYPE)
    inline CVar<VALUE_TYPE>& operator=(VALUE_TYPE value) {
        // If min=max, no clamping, just assign
        auto old_value = m_value;
        m_value = (m_min == m_max) ? value : clampValue(value);
        if (m_watch_callback && old_value != m_value) {
            m_watch_callback(this->value());
        }
        return *this;
    }
    inline CVar<c_cstr>& operator=(c_cstr value) {
        m_storage = value;
        m_value = m_storage.c_str();
        return *this;
    }

    // Transparent cast to value type
    inline operator VALUE_TYPE(void) const { return m_value; }

    // Explicit value cast
    inline VALUE_TYPE& value(void) { return m_value; }

    // Register a callback to take some action when this setting is changed.
    // NB. Only one such callback may be registered per cvar.
    inline void watch(std::function<void(VALUE_TYPE)> const& callback) {
        m_watch_callback = callback;
    }

    static CVar<VALUE_TYPE>* find(c_cstr name);

private:
    inline VALUE_TYPE clampValue(VALUE_TYPE value) {
        if (value < m_min) {
            //DEBUGTODO: Emit debug event here
            value = m_min;
        } else if (value > m_max) {
            //DEBUGTODO: Emit debug event here
            value = m_max;
        }
        return value;
    }

    void addToList(void);
    CVar<VALUE_TYPE>* m_next;

    c_cstr m_name;
    c_cstr m_synopsis;
    VALUE_TYPE m_value;
    decltype(m_value) m_min;
    decltype(m_value) m_max;
    std::function<void(VALUE_TYPE)> m_watch_callback;
    std::string m_storage;
};

/* Forward declarations of explicit specializations */
template<> CVar_f* CVar_f::find(c_cstr name);
template<> CVar_i* CVar_i::find(c_cstr name);
template<> CVar_b* CVar_b::find(c_cstr name);
template<> CVar_s* CVar_s::find(c_cstr name);


/* Locate a cvar by name and return its value by reference */
// template<typename T>
// inline T& FINDCV(c_cstr some_cvar_name, T default_value);

inline i64& FINDCVi(c_cstr some_cvar_name, i64&& default_value) {
    CVar_i* ptr = CVar_i::find(some_cvar_name);
    if (ptr && *ptr) return ptr->value();
    else             return default_value;
}
inline f64& FINDCVf(c_cstr some_cvar_name, f64&& default_value) {
    CVar_f* ptr = CVar_f::find(some_cvar_name);
    if (ptr && *ptr) return ptr->value();
    else             return default_value;
}
inline bool& FINDCVb(c_cstr some_cvar_name, bool&& default_value) {
    CVar_b* ptr = CVar_b::find(some_cvar_name);
    if (ptr && *ptr) return ptr->value();
    else             return default_value;
}
inline c_cstr& FINDCVs(c_cstr some_cvar_name,
                       c_cstr&& default_value) {
    CVar_s* ptr = CVar_s::find(some_cvar_name);
    if (ptr && *ptr) return ptr->value();
    else             return default_value;
}

/* Convenience function for toggling boolean cvars from pointers */
inline bool SCV_TOGGLE(CVar_b *cv_ptr) {
    if (cv_ptr != NULL) {
        *cv_ptr = !CV(*cv_ptr);
        return CV(*cv_ptr);
    }
    return false;
}

