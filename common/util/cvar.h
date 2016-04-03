/* Configuration Variables
   =======================

   DSS/DATA allocated configuration variable list. Can be freely added
   anywhere in one line, and queried in other compilation units either via
   extern or by-name lookup. Can also be watched by a single std::function.
*/


#include <functional>

#define CV(some_cvar) ((some_cvar).value())
#define SCV(some_cvar_p) ((some_cvar_p != NULL) ? some_cvar_p->value() : NULL)

// TODO: config file generator from defined CVars

template<typename VALUE_TYPE>
class CVar {
public:
    CVar(char const* name, char const* synopsis, VALUE_TYPE value)
        : m_next           ( nullptr  )
        , m_name           ( name     )
        , m_synopsis       ( synopsis )
        , m_value          ( value    )
        , m_min            ( value    )
        , m_max            ( value    )
        , m_watch_callback ( nullptr  )
    {
        addToList();
    }
    CVar(char const* name,
         char const* synopsis,
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
    {
        addToList();
    }

    // Transparent assignment as VALUE_TYPE
    inline CVar<VALUE_TYPE>& operator=(VALUE_TYPE value) {
        // If min=max, no clamping, just assign
        auto old_value = m_value;
        m_value = (m_min == m_max) ? value : clampValue(value);
        if (m_watch_callback && old_value != m_value) {
            m_watch_callback(this->value());
        }
        return *this;
    }

    // Transparent cast to value type
    inline operator VALUE_TYPE(void) const { return m_value; }

    // Explicit value cast
    inline VALUE_TYPE value(void) const { return m_value; }

    // Register a callback to take some action when this setting is changed.
    // NB. Only one such callback may be registered per cvar.
    inline void watch(std::function<void(VALUE_TYPE)> const& callback) {
        m_watch_callback = callback;
    }

    static CVar<VALUE_TYPE>* find(char const* name);

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

    char const* m_name;
    char const* m_synopsis;
    VALUE_TYPE m_value;
    decltype(m_value) m_min;
    decltype(m_value) m_max;
    std::function<void(VALUE_TYPE)> m_watch_callback;
};

class CVar_s {
public:
    CVar_s(char const* name, char const* synopsis, char const* value)
          : m_next     ( nullptr  )
          , m_name     ( name     )
          , m_synopsis ( synopsis )
          , m_value    ( value    )
    {
        addToList();
    }

    // Transparent assignment as bool
    inline CVar_s& operator=(char const* value) {
        auto old_value = m_value;
        m_value = value;
        if (m_watch_callback && m_value != old_value) {
            m_watch_callback(this->value());
        }
        return *this;
    }

    // Transparent cast to bool
    inline operator char const*(void) const { return m_value.c_str(); }

    // Explicit value cast
    inline char const* value(void) const { return m_value.c_str(); }

    // Register a callback to take some action when this setting is changed.
    // NB. Only one such callback may be registered per cvar.
    inline void watch(std::function<void(char const*)> const& callback) {
        m_watch_callback = callback;
    }

    static CVar_s* find(char const* name);

private:
    DISALLOW_COPY_AND_ASSIGN(CVar_s);

    void addToList(void);
    CVar_s* m_next;

    char const* m_name;
    char const* m_synopsis;
    std::string m_value;
    std::function<void(char const*)> m_watch_callback;
};

typedef CVar<double> CVar_f;
typedef CVar<int64_t> CVar_i;
typedef CVar<bool> CVar_b;

/* Convenience function for toggling boolean cvars from pointers */
inline bool SCV_TOGGLE(CVar_b *cv_ptr) {
    if (cv_ptr != NULL) {
        *cv_ptr = !CV(*cv_ptr);
        return CV(*cv_ptr);
    }
    return false;
}

