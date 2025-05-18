#pragma once

#include <cstddef>
#include <type_traits>

template <size_t N, size_t M, typename T>
  requires(std::is_arithmetic_v<T>)
struct mat;
