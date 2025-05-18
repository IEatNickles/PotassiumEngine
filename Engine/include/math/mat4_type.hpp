#pragma once

#include "mat.hpp"
#include "vec.hpp"

template <typename T> struct mat<4, 4, T> {
  vec<4, T> value[4];

  constexpr mat() : value({0}, {0}, {0}, {0}) {}

  constexpr mat(T m00, T m01, T m02, T m03, //
                T m10, T m11, T m12, T m13, //
                T m20, T m21, T m22, T m23, //
                T m30, T m31, T m32, T m33)
      : value({m00, m01, m02, m03}, //
              {m10, m11, m12, m13}, //
              {m20, m21, m22, m23}, //
              {m30, m31, m32, m33}) {}

  constexpr mat(vec<4, T> m0, vec<4, T> m1, vec<4, T> m2, vec<4, T> m3)
      : value(m0, m1, m2, m3) {}

  constexpr mat(mat const &m) = default;

  constexpr mat operator+(mat const &m) const {
    return mat(value[0] + m[0], value[1] + m[1], value[2] + m[2],
               value[3] + m[3]);
  }
  constexpr mat operator-(mat const &m) const {
    return mat(value[0] - m[0], value[1] - m[1], value[2] - m[2],
               value[3] - m[3]);
  }
  constexpr mat operator*(mat const &m) const {
    mat mT = m.transpose();
    mat r;

    r[0][0] = vec<4, T>::dot(value[0], mT[0]);
    r[0][1] = vec<4, T>::dot(value[0], mT[1]);
    r[0][2] = vec<4, T>::dot(value[0], mT[2]);
    r[0][3] = vec<4, T>::dot(value[0], mT[3]);

    r[1][0] = vec<4, T>::dot(value[1], mT[0]);
    r[1][1] = vec<4, T>::dot(value[1], mT[1]);
    r[1][2] = vec<4, T>::dot(value[1], mT[2]);
    r[1][3] = vec<4, T>::dot(value[1], mT[3]);

    r[2][0] = vec<4, T>::dot(value[2], mT[0]);
    r[2][1] = vec<4, T>::dot(value[2], mT[1]);
    r[2][2] = vec<4, T>::dot(value[2], mT[2]);
    r[2][3] = vec<4, T>::dot(value[2], mT[3]);

    r[3][0] = vec<4, T>::dot(value[3], mT[0]);
    r[3][1] = vec<4, T>::dot(value[3], mT[1]);
    r[3][2] = vec<4, T>::dot(value[3], mT[2]);
    r[3][3] = vec<4, T>::dot(value[3], mT[3]);

    return r;
  }

  constexpr mat operator*(T s) const {
    return mat(value[0] * s, value[1] * s, value[2] * s, value[3] * s);
  }

  constexpr mat operator/(T s) const {
    return mat(value[0] / s, value[1] / s, value[2] / s, value[3] / s);
  }

  constexpr mat transpose() const {
    return mat(value[0][0], value[1][0], value[2][0], value[3][0], //
               value[0][1], value[1][1], value[2][1], value[3][1], //
               value[0][2], value[1][2], value[2][2], value[3][2], //
               value[0][3], value[1][3], value[2][3], value[3][3]);
  }

  // Calculates the determinant of a matrix.
  //
  // @return The determinant of a matrix.
  constexpr T det() const {
    T a00 = value[0][0];
    T a01 = value[0][1];
    T a02 = value[0][2];
    T a03 = value[0][3];

    T a10 = value[1][0];
    T a11 = value[1][1];
    T a12 = value[1][2];
    T a13 = value[1][3];

    T a20 = value[2][0];
    T a21 = value[2][1];
    T a22 = value[2][2];
    T a23 = value[2][3];

    T a30 = value[3][0];
    T a31 = value[3][1];
    T a32 = value[3][2];
    T a33 = value[3][3];

    return (a00 * (a11 * (a22 * a33 - a32 * a23)) +
            (a21 * (a12 * a33 - a32 * a13)) - (a31 * (a12 * a23 - a22 * a13))) +
           (a10 * (a01 * (a22 * a33 - a32 * a23)) +
            (a21 * (a02 * a33 - a32 * a03)) - (a31 * (a02 * a23 - a22 * a03))) -
           (a20 * (a01 * (a12 * a33 - a32 * a13)) +
            (a11 * (a02 * a33 - a32 * a03)) - (a31 * (a02 * a13 - a12 * a03))) +
           (a30 * (a01 * (a12 * a23 - a22 * a13)) +
            (a11 * (a02 * a23 - a22 * a03)) - (a21 * (a02 * a13 - a12 * a03)));
  }

  // Calculates the inverse of a matrix.
  //
  // @return The inverse of a matrix.
  // Shout out g-truc
  constexpr mat inverse() const {
    // Shout out to g-truc for this crazyness, ain't no way i'm typin' all that
    //   https://github.com/g-truc/glm

    T Coef00 = value[2][2] * value[3][3] - value[3][2] * value[2][3];
    T Coef02 = value[1][2] * value[3][3] - value[3][2] * value[1][3];
    T Coef03 = value[1][2] * value[2][3] - value[2][2] * value[1][3];

    T Coef04 = value[2][1] * value[3][3] - value[3][1] * value[2][3];
    T Coef06 = value[1][1] * value[3][3] - value[3][1] * value[1][3];
    T Coef07 = value[1][1] * value[2][3] - value[2][1] * value[1][3];

    T Coef08 = value[2][1] * value[3][2] - value[3][1] * value[2][2];
    T Coef10 = value[1][1] * value[3][2] - value[3][1] * value[1][2];
    T Coef11 = value[1][1] * value[2][2] - value[2][1] * value[1][2];

    T Coef12 = value[2][0] * value[3][3] - value[3][0] * value[2][3];
    T Coef14 = value[1][0] * value[3][3] - value[3][0] * value[1][3];
    T Coef15 = value[1][0] * value[2][3] - value[2][0] * value[1][3];

    T Coef16 = value[2][0] * value[3][2] - value[3][0] * value[2][2];
    T Coef18 = value[1][0] * value[3][2] - value[3][0] * value[1][2];
    T Coef19 = value[1][0] * value[2][2] - value[2][0] * value[1][2];

    T Coef20 = value[2][0] * value[3][1] - value[3][0] * value[2][1];
    T Coef22 = value[1][0] * value[3][1] - value[3][0] * value[1][1];
    T Coef23 = value[1][0] * value[2][1] - value[2][0] * value[1][1];

    vec<4, T> Fac0(Coef00, Coef00, Coef02, Coef03);
    vec<4, T> Fac1(Coef04, Coef04, Coef06, Coef07);
    vec<4, T> Fac2(Coef08, Coef08, Coef10, Coef11);
    vec<4, T> Fac3(Coef12, Coef12, Coef14, Coef15);
    vec<4, T> Fac4(Coef16, Coef16, Coef18, Coef19);
    vec<4, T> Fac5(Coef20, Coef20, Coef22, Coef23);

    vec<4, T> Vec0(value[1][0], value[0][0], value[0][0], value[0][0]);
    vec<4, T> Vec1(value[1][1], value[0][1], value[0][1], value[0][1]);
    vec<4, T> Vec2(value[1][2], value[0][2], value[0][2], value[0][2]);
    vec<4, T> Vec3(value[1][3], value[0][3], value[0][3], value[0][3]);

    vec<4, T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
    vec<4, T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
    vec<4, T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
    vec<4, T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

    vec<4, T> SignA(+1, -1, +1, -1);
    vec<4, T> SignB(-1, +1, -1, +1);
    mat<4, 4, T> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA,
                         Inv3 * SignB);

    vec<4, T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

    vec<4, T> Dot0(value[0] * Row0);
    T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

    T OneOverDeterminant = static_cast<T>(1) / Dot1;

    return Inverse * OneOverDeterminant;
  }

  static constexpr mat perspective(T fovy, T aspect, T near, T far) {
    T half_fov = fovy / static_cast<T>(2);
  }

  constexpr vec<4, T> &operator[](int i) { return value[i]; }
  constexpr vec<4, T> const &operator[](int i) const { return value[i]; }
};
