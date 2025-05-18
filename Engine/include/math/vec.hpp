#pragma once

#include <cstddef>
#include <type_traits>

template <size_t N, typename T>
  requires(std::is_arithmetic_v<T>)
struct vec;
