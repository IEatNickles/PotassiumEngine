#pragma once

#include "vec.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <ostream>

template <typename T> struct vec<3, T> {
  // clang-format off
  // Shout out g-truc for the "idea".
  union {
    struct { T x, y, z; };
    struct { T r, g, b; };
    struct { T s, t, p; };
  };
  // clang-format on

  constexpr vec<3, T>() : x(0), y(0), z(0) {}
  constexpr vec<3, T>(T x, T y, T z) : x(x), y(y), z(z) {}
  constexpr vec<3, T>(T s) : x(s), y(s), z(s) {}
  constexpr vec<3, T>(vec<2, T> const &v, T z, T w) : x(v.x), y(v.y), z(z) {}
  constexpr vec<3, T>(vec<2, T> const &v, T z) : x(v.x), y(v.y), z(z) {}
  constexpr vec<3, T>(T x, vec<2, T> const &v) : x(x), y(v.x), z(v.y) {}
  constexpr vec<3, T>(vec<3, T> const &v) = default;

  template <typename Tx, typename Ty, typename Tz>
  constexpr vec<3, T>(Tx x, Ty y, Tz z)
      : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}
  template <typename S>
  constexpr vec<3, T>(S s)
      : x(static_cast<T>(s)), y(static_cast<T>(s)), z(static_cast<S>(s)) {}
  template <typename R>
  constexpr vec<3, T>(vec<3, R> const &v)
      : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {
  }

  constexpr vec<3, T> &operator=(vec<3, T> const &v) const = default;

  constexpr bool operator==(vec<3, T> const &v) const {
    return x == v.x && y == v.y && z == v.z;
  }
  constexpr bool operator!=(vec<3, T> const &v) const {
    return x != v.x && y != v.y && z != v.z;
  }

  constexpr vec<3, T> operator+(vec<3, T> const &v) const {
    return vec(x + v.x, y + v.y, z + v.z);
  }
  constexpr vec<3, T> operator-(vec<3, T> const &v) const {
    return vec(x - v.x, y - v.y, z - v.z);
  }
  constexpr vec<3, T> operator*(vec<3, T> const &v) const {
    return vec(x * v.x, y * v.y, z * v.z);
  }
  constexpr vec<3, T> operator/(vec<3, T> const &v) const {
    return vec(x / v.x, y / v.y, z / v.z);
  }

  constexpr vec<3, T> operator*(T s) const {
    return vec<3, T>(x * s, y * s, z * s);
  }

  /* @return The magnitude, or length, of the vector. */
  constexpr T mag() const { return std::sqrt(x * x + y * y + z * z); }
  /* @return The magnitude, or length, of the vector, squared. */
  constexpr T mag2() const { return x * x + y * y + z * z; }

  /* @return The angle between the vectors a and b. */
  static constexpr T angle(vec<3, T> const &a, vec<3, T> const &b) {
    return std::atan2(cross(a, b).mag(), dot(a, b));
  }

  /* @return The cross product of a and b. */
  static constexpr vec<3, T> cross(vec<3, T> const &a, vec<3, T> const &b) {
    // clang-format off
    return vec<3, T>(
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x));
    // clang-format on
  }

  /* @return The dot of two vectors. */
  static constexpr T dot(vec<3, T> const &a, vec<3, T> const &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  /* @return The distance between a and b. */
  static constexpr T distance(vec<3, T> const &a, vec<3, T> const &b) {
    return (a - b).mag();
  }

  /* @return The distance between a and b, squared. */
  static constexpr T distance2(vec<3, T> const &a, vec<3, T> const &b) {
    return (a - b).mag2();
  }

  /* @return The *nth* element of the vector. */
  constexpr T &operator[](size_t n) {
    assert(n < 3);
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    }
  }

  /* @return The *nth* element of the vector. */
  constexpr T const &operator[](size_t n) const {
    assert(n < 3);
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    }
  }
};

template <typename T>
std::ostream &operator<<(std::ostream &out, vec<3, T> const &v) {
  return out << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
}

// Thank's g-truc
template <typename T> struct std::hash<vec<3, T>> {
  constexpr size_t operator()(vec<3, T> const &v) const {
    size_t seed = 0;

    size_t hash = std::hash<T>(v.x);
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
    hash = std::hash<T>(v.y);
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
    hash = std::hash<T>(v.z);
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;

    return seed;
  }
};
