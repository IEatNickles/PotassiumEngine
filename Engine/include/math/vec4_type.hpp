#pragma once

#include "vec.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <ostream>

template <typename T> struct vec<4, T> {
  // clang-format off
  // Shout out g-truc for the "idea".
  union {
    struct { T x, y, z, w; };
    struct { T r, g, b, a; };
    struct { T s, t, p, q; };
  };
  // clang-format on

  constexpr vec<4, T>() : x(0), y(0), z(0), w(0) {}
  constexpr vec<4, T>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
  constexpr vec<4, T>(T s) : x(s), y(s), z(s), w(s) {}
  constexpr vec<4, T>(vec<2, T> const &v, T z, T w)
      : x(v.x), y(v.y), z(z), w(w) {}
  constexpr vec<4, T>(T x, vec<2, T> const &v, T w)
      : x(x), y(v.x), z(v.y), w(w) {}
  constexpr vec<4, T>(T x, T y, vec<2, T> const &v)
      : x(x), y(y), z(v.x), w(v.y) {}
  constexpr vec<4, T>(vec<2, T> const &v0, vec<2, T> const &v1)
      : x(v0.x), y(v0.y), z(v1.x), w(v1.y) {}
  constexpr vec<4, T>(vec<4, T> const &v) = default;

  template <typename Tx, typename Ty, typename Tz, typename Tw>
  constexpr vec<4, T>(Tx x, Ty y, Tz z, Tw w)
      : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)),
        w(static_cast<T>(w)) {}
  template <typename S>
  constexpr vec<4, T>(S s)
      : x(static_cast<T>(s)), y(static_cast<T>(s)), z(static_cast<S>(s)),
        w(static_cast<T>(s)) {}
  template <typename R>
  constexpr vec<4, T>(vec<4, R> const &v)
      : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)),
        w(static_cast<T>(v.w)) {}

  constexpr vec<4, T> &operator=(vec<4, T> const &v) const = default;

  constexpr bool operator==(vec<4, T> const &v) const {
    return x == v.x && y == v.y && z == v.z && w == v.w;
  }
  constexpr bool operator!=(vec<4, T> const &v) const {
    return x != v.x && y != v.y && z != v.z && w != v.w;
  }

  constexpr vec<4, T> operator+(vec<4, T> const &v) const {
    return vec(x + v.x, y + v.y, z + v.z, w + v.w);
  }
  constexpr vec<4, T> operator-(vec<4, T> const &v) const {
    return vec(x - v.x, y - v.y, z - v.z, w - v.w);
  }
  constexpr vec<4, T> operator*(vec<4, T> const &v) const {
    return vec(x * v.x, y * v.y, z * v.z, w * v.w);
  }
  constexpr vec<4, T> operator/(vec<4, T> const &v) const {
    return vec(x / v.x, y / v.y, z / v.z, w / v.w);
  }

  constexpr vec<4, T> operator*(T s) const {
    return vec<4, T>(x * s, y * s, z * s, w * s);
  }

  /* @return The magnitude, or length, of the vector. */
  constexpr T mag() const { return std::sqrt(x * x + y * y + z * z + w * w); }
  /* @return The magnitude, or length, of the vector, squared. */
  constexpr T mag2() const { return x * x + y * y + z * z + w * w; }

  /* @return The dot of two vectors. */
  static constexpr T dot(vec<4, T> const &a, vec<4, T> const &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }

  /* @return The distance between a and b. */
  static constexpr T distance(vec<4, T> const &a, vec<4, T> const &b) {
    return (a - b).mag();
  }

  /* @return The distance between a and b, squared. */
  static constexpr T distance2(vec<4, T> const &a, vec<4, T> const &b) {
    return (a - b).mag2();
  }

  /* @return The *nth* element of the vector. */
  constexpr T &operator[](size_t n) {
    assert(n < 4);
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
    }
  }

  /* @return The *nth* element of the vector. */
  constexpr T const &operator[](size_t n) const {
    assert(n < 4);
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
    }
  }
};

template <typename T>
std::ostream &operator<<(std::ostream &out, vec<4, T> const &v) {
  return out << "[ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w
             << " ]";
}

// Thank's g-truc
template <typename T> struct std::hash<vec<4, T>> {
  constexpr size_t operator()(vec<4, T> const &v) const {
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
    hash = std::hash<T>(v.w);
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;

    return seed;
  }
};
