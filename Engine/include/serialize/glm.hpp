#pragma once

#include "Log.hpp"
#include "glm/glm.hpp"
#include "nlohmann/json.hpp"

namespace glm {
template <typename T, glm::qualifier Q>
void to_json(nlohmann::json &j, tvec2<T, Q> const &v) {
  j[0] = static_cast<T>(v.x);
  j[1] = static_cast<T>(v.y);
}
template <typename T, qualifier Q>
void from_json(nlohmann::json const &j, tvec2<T, Q> &v) {
  DEBUG_ASSERT(j.is_array() && j.size() <= 2,
               "Vector 2 must be an array of length 2 or less");
  v.x = j.size() >= 1 ? j[0].get<T>() : static_cast<T>(0);
  v.y = j.size() >= 2 ? j[1].get<T>() : static_cast<T>(0);
}

template <typename T, qualifier Q>
void to_json(nlohmann::json &j, tvec3<T, Q> const &v) {
  j[0] = static_cast<T>(v.x);
  j[1] = static_cast<T>(v.y);
  j[2] = static_cast<T>(v.z);
}
template <typename T, qualifier Q>
void from_json(nlohmann::json const &j, tvec3<T, Q> &v) {
  DEBUG_ASSERT(j.is_array() && j.size() <= 3,
               "Vector 3 must be an array of length 3 or less");
  v.x = j.size() >= 1 ? j[0].get<T>() : static_cast<T>(0);
  v.y = j.size() >= 2 ? j[1].get<T>() : static_cast<T>(0);
  v.z = j.size() >= 3 ? j[2].get<T>() : static_cast<T>(0);
}

template <typename T, qualifier Q>
void to_json(nlohmann::json &j, tvec4<T, Q> const &v) {
  j[0] = static_cast<T>(v.x);
  j[1] = static_cast<T>(v.y);
  j[2] = static_cast<T>(v.z);
  j[3] = static_cast<T>(v.w);
}
template <typename T, qualifier Q>
void from_json(nlohmann::json const &j, tvec4<T, Q> &v) {
  DEBUG_ASSERT(j.is_array() && j.size() <= 4,
               "Vector 4 must be an array of length 4 or less");
  v.x = j.size() >= 1 ? j[0].get<T>() : static_cast<T>(0);
  v.y = j.size() >= 2 ? j[1].get<T>() : static_cast<T>(0);
  v.z = j.size() >= 3 ? j[2].get<T>() : static_cast<T>(0);
  v.w = j.size() >= 4 ? j[3].get<T>() : static_cast<T>(0);
}

template <typename T, qualifier Q>
void to_json(nlohmann::json &j, qua<T, Q> const &v) {
  j[0] = static_cast<T>(v.x);
  j[1] = static_cast<T>(v.y);
  j[2] = static_cast<T>(v.z);
  j[3] = static_cast<T>(v.w);
}
template <typename T, qualifier Q>
void from_json(nlohmann::json const &j, qua<T, Q> &v) {
  DEBUG_ASSERT(j.is_array() && j.size() <= 4,
               "quaternion must be an array of length 4 or less");
  v.x = j.size() >= 1 ? j[0].get<T>() : static_cast<T>(0);
  v.y = j.size() >= 2 ? j[1].get<T>() : static_cast<T>(0);
  v.z = j.size() >= 3 ? j[2].get<T>() : static_cast<T>(0);
  v.w = j.size() >= 4 ? j[3].get<T>() : static_cast<T>(0);
}
} // namespace glm
