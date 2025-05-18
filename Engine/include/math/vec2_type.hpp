#pragma once

#include "vec.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <ostream>

template <typename T> struct vec<2, T> {
  // clang-format off
  // Shout out g-truc for the "idea".
  union {
    struct { T x, y; };
    struct { T r, g; };
    struct { T s, t; };
  };
  // clang-format on

  constexpr vec<2, T>() : x(0), y(0) {}
  constexpr vec<2, T>(T x, T y) : x(x), y(y) {}
  constexpr vec<2, T>(T s) : x(s), y(s) {}
  constexpr vec<2, T>(vec<2, T> const &v) = default;

  template <typename Tx, typename Ty>
  constexpr vec<2, T>(Tx x, Ty y)
      : x(static_cast<T>(x)), y(static_cast<T>(y)) {}
  template <typename S>
  constexpr vec<2, T>(S s) : x(static_cast<T>(s)), y(static_cast<T>(s)) {}
  template <typename R>
  constexpr vec<2, T>(vec<2, R> const &v)
      : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

  constexpr vec<2, T> &operator=(vec<2, T> const &v) const = default;

  constexpr bool operator==(vec<2, T> const &v) const {
    return x == v.x && y == v.y;
  }
  constexpr bool operator!=(vec<2, T> const &v) const {
    return x != v.x && y != v.y;
  }

  constexpr vec<2, T> operator+(vec<2, T> const &v) const {
    return vec(x + v.x, y + v.y);
  }
  constexpr vec<2, T> operator-(vec<2, T> const &v) const {
    return vec(x - v.x, y - v.y);
  }
  constexpr vec<2, T> operator*(vec<2, T> const &v) const {
    return vec(x * v.x, y * v.y);
  }
  constexpr vec<2, T> operator/(vec<2, T> const &v) const {
    return vec(x / v.x, y / v.y);
  }

  constexpr vec<2, T> operator*(T s) const { return vec<2, T>(x * s, y * s); }

  /* @return The magnitude, or length, of the vector. */
  constexpr T mag() const { return std::sqrt(x * x + y * y); }
  /* @return The magnitude, or length, of the vector, squared. */
  constexpr T mag2() const { return x * x + y * y; }

  /* @return The angle between the vectors a and b. */
  static constexpr T angle(vec<2, T> const &a) { return std::atan2(a.y, a.x); }

  /* @return The angle between the vectors a and b. */
  static constexpr T angle(vec<2, T> const &a, vec<2, T> const &b) {
    return std::atan2(a.y, a.x) - std::atan2(b.y, b.x);
  }

  /* @return The dot of two vectors. */
  static constexpr T dot(vec<2, T> const &a, vec<2, T> const &b) {
    return a.x * b.x + a.y * b.y;
  }

  /* @return The distance between a and b. */
  static constexpr T distance(vec<2, T> const &a, vec<2, T> const &b) {
    return (a - b).mag();
  }

  /* @return The distance between a and b, squared. */
  static constexpr T distance2(vec<2, T> const &a, vec<2, T> const &b) {
    return (a - b).mag2();
  }

  /* @return The *nth* element of the vector. */
  constexpr T &operator[](size_t n) {
    assert(n < 2);
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    }
  }

  /* @return The *nth* element of the vector. */
  constexpr T const &operator[](size_t n) const {
    assert(n < 2);
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    }
  }
};

template <typename T>
std::ostream &operator<<(std::ostream &out, vec<2, T> const &v) {
  return out << "[ " << v.x << ", " << v.y << ", " << " ]";
}

// Thank's g-truc
template <typename T> struct std::hash<vec<2, T>> {
  constexpr size_t operator()(vec<2, T> const &v) const {
    size_t seed = 0;

    size_t hash = std::hash<T>(v.x);
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
    hash = std::hash<T>(v.y);
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;

    return seed;
  }
};
