/* Game Entities
   =============

   Defines the enclosing game entity type.
*/

#pragma once

/* Defines the base class for all component types */
template<typename T>
class ComponentState {
private:
  ENFORCE_POD(T);
public:
  Pool<T, true> m_instances;

  /* Metadata */
  const char* name() { return "GenericComponent"; }
  u64 total_bytes() { return m_instances.total_bytes() + sizeof(*this); }

  /* Entity Lifecycle */

  // Create a new component of this type, and associated with an entity ID
  template<typename ...ctor_arg_types>
  ID create(ID entity_id, ctor_arg_types && ... _ctor_args) {
    return m_instances.create_at(entity_id,
                                 std::forward<ctor_arg_types>(_ctor_args)...);
  }

  // Destroy an existing component of this type (no-op on double-free)
  void destroy(ID entity_id) {
    m_instances.destroy(entity_id);
  }

  // Look up the component instance corresponding to a given entity
  T& lookup(ID entity_id) const {
    if (m_instances.contains(entity_id)) {
      return m_instances[entity_id];
    } else {
      return T::defaultInstance;
    }
  }
  T& operator[](ID entity_id) const {
    return this->lookup(entity_id);
  }
};

/* Defines a macro used to fill out all the boilerplate for each component type
   as it's defined */
#define COMPONENT(name)                                                       \
    struct name;                                                              \
    using name##State = ComponentState< name >;                               \
    void beginPlay(name##State &state);                                       \
    void tick(name##State &state, u32 dt, u64 frame);                         \
    void postTick(name##State&, u32 dt, u64 frame);                           \
    void blendView(name##State const& prev, name##State& next, f32);          \
    struct name
