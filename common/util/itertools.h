/* Iteration Tools
 * ===============
 *
 * C++11/14 added great automatic deduction and iteration facilities, but it's
 * often missing little conveniences to make the task of using them as simple
 * as it could be. */

#pragma once

/* Lazily yield Ts in the range provided. Emulates python 3's range() */
template <typename T>
class range_t {
public:
  /* named start / stop to avoid colliding with begin() / end() */
  T start, stop, step;

  inline range_t(T end) : start(0), stop(end), step(1) { }
  inline range_t(T begin, T end, T step=1)
    : start(begin)
    , stop(end)
    , step(step) { }

  class iterator {
  public:
    typedef std::output_iterator_tag iterator_category;

    range_t const& r;
    T value;

    inline iterator(range_t const& r, T value) : r(r), value(value) { }
    inline iterator(iterator const& i) : r(i.r), value(i.value) { }
    inline iterator& operator=(iterator const& i) {
      if (this != &i) {
        this->r = i.r;
        this->value = i.value;
      }
    }

    inline T operator*() { return value; }

    inline bool operator==(iterator const& i) { return value == i.value; }
    inline bool operator!=(iterator const& i) { return value != i.value; }

    inline iterator& operator++() { value += r.step; return *this; }
    inline iterator operator++(int) {
      iterator ret(r, value);
      value += r.step;
      return ret;
    }

    inline iterator& operator--() { value -= r.step; return *this; }
    inline iterator operator--(int) {
      iterator ret(r, value);
      value -= r.step;
      return ret;
    }

    inline iterator& operator+=(T steps) {
      value += (steps * r.step);
      return *this;
    }
    inline iterator& operator-=(T steps) {
      value -= (steps * r.step);
      return *this;
    }
  };

  inline iterator begin() const { return range_t<T>::iterator(*this, start); }
  inline iterator end() const { return range_t<T>::iterator(*this, stop); }
};

template<typename T>
range_t<T> range(T begin, T end, T step = 1) {
  return range_t<T>(begin, end, step);
}
template<typename T>
range_t<T> range(T end) { return range_t<T>(0, end, 1); }
