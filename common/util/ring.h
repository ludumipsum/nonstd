/* Ring-Buffer
   ===========
   Basic object ring-buffer based on Region<T>
*/
#pragma once
#include "region.h"
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <string>
#include <assert.h>


//TODO: Come up with a better name than "bounds checked"
//TODO: Consider expanding the range of behaviors when pushing when `used ==
// capacity`. Currently, we destroy and re-create. Should we have the option to
// bounds check and error? The option to resize?
template<typename T, bool BoundsChecked=false>
class Ring {
public:
    Ring(u64 count, char const * _name) :
        _object_name(std::string(_name) + "/Objects"),
        _objects(count, this->_object_name.c_str()),
        _name(_name),
        _head(0),
        _tail(0),
        _used(0) {
            if (count == 0) {
                #if defined(DEBUG_MEMORY)
                    LOG("Ring \"%s\" was created with `count == 0`. Zero-sized "
                        "Rings are not supported, so count has been set to 1",
                        this->_name);
                #endif
                count = 1;
                // _objects will have already bumped from 0 to 1
            }
    }
    Ring(Ring const& other) :
        _object_name(other._object_name),
        _objects(other._objects),
        _name(other._name),
        _head(other._head),
        _tail(other._tail),
        _used(other._used) { }
    Ring& operator=(Ring const& other) {
        this->_object_name = other._object_name;
        this->_objects = other._objects;
        this->_name = other._name;
        this->_head = other._head;
        this->_tail = other._tail;
        this->_used = other._used;
    }
    Ring(Ring&& other) :
        _object_name(std::move(other._object_name)),
        _objects(std::move(other._objects)),
        _name(std::move(other._name)),
        _head(other._head),
        _tail(other._tail),
        _used(other._used) { }
    Ring& operator=(Ring&& other) {
        this->_object_name = std::move(other._object_name);
        this->_objects = std::move(other._objects);
        this->_name = std::move(other._name);
        this->_head = other._head;
        this->_tail = other._tail;
        this->_used = other._used;
    }
    ~Ring() = default;

public:
    std::string  _object_name;
    Region<T>    _objects;
    char const * _name;
    u64          _head;
    u64          _tail;
    u64          _used;

    inline char const * name()     { return this->_name; }
    inline u64          used()     { return this->_used; }
    inline u64          capacity() { return this->_objects.capacity(); }

    inline u64    used_bytes()     { return (this->_used * sizeof(T)); }
    inline u64    capacity_bytes() { return this->_objects.capacity_bytes(); }


    template<typename ...ctor_arg_types>
    inline T& push(ctor_arg_types && ... _ctor_args) {
        if (this->_used == this->capacity()) {
            this->_placement_delete(this->_objects[this->_tail]);
            // Look at me. I can write one-liners. I'm so good at computers.
            if (++(this->_tail) >= this->capacity()) { this->_tail = 0; }
        } else {
            this->_used++;
        }
        T& ret = this->_objects.emplace(this->_head,
                                std::forward<ctor_arg_types>(_ctor_args)...);
        if (++(this->_head) >= this->capacity()) { this->_head = 0; }
        return ret;
    }

    inline void pop() {
        if (this->_used == 0) { return; }
        this->_placement_delete(this->_objects[this->_tail]);
        if (++(this->_tail) >= this->capacity()) { this->_tail = 0; }
        this->_used--;
    }

    inline T& peek(u64 index=0) {
        u64 target = index + this->_tail;
        if (target >= this->capacity()) { target -= this->capacity(); }
        #if defined(DEBUG_MEMORY)
            if (!this->_is_active(target)) {
                CRASH(EFAULT, "Memory Ring bounds-check failed for %s; index %"
                              PRIu64 " (targeting: %" PRIu64 ", head: %" PRIu64
                              ", tail: %" PRIu64 ") is not active.",
                              this->_name, index, target,
                              this->_head, this->_tail);
            }
        #endif
        return this->_objects[target];
    }
    inline T& latest() { return peek(this->_used-1); }
    inline T& operator[](u64 index) { return peek(index); }

    // If the object is POD, this function is a noop.
    TEMPLATE_ENABLE(std::is_pod<T>::value, T)
    inline void _placement_delete(T& object) {
        UNUSED(object);
    }

    // If it's not POD, manually call the destructor.
    TEMPLATE_ENABLE(!std::is_pod<T>::value, T)
    inline void _placement_delete(T& object) {
        object.~T();
    }

    inline bool _is_active(u64 index) {
        if (this->_tail < this->_head) {
            return ((index >= this->_tail) && (index <= this->_head));
        } else if (this->_tail > this->_head) {
            return (!(index >= this->_tail) != !(index <= this->_head));
        } else /* (this->_head == this->_tail) */ {
            return (this->_used != 0);
        }
    }

    //### Iterator magic
    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Ring<T> * _ring;
        u64 _index;

        iterator(Ring<T> * ring, u64 index=0) :
            _ring(ring),
            _index(index) {}

        inline bool operator==(const iterator& other) const {
            return (this->_index == other._index);
        }
        inline bool operator!=(const iterator& other) const {
            return (this->_index != other._index);
        }

        inline iterator& operator++() {
            this->_index++;
            return *this;
        }
        inline iterator operator++(int) {
            auto ret = *this + 1;
            this->_index++;
            return ret;
        }
        template<typename T>
        inline iterator& operator+=(T n) {
            this->_index = (this->_index + n) % this->_ring.used();
            return *this;
        }
        template<typename T>
        inline iterator operator+(T n) {
            iterator ret(this->_ring, this->_index);
            ret._index += n;
            return ret;
        }

        inline T& operator*() const { return this->_ring->peek(this->_index); }
    };

    inline iterator begin() { return Ring::iterator{this, 0}; }
    inline iterator end()   { return Ring::iterator{this, this->used()}; }
};
