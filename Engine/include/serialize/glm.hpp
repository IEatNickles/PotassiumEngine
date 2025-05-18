#pragma once

#include "Log.hpp"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

template <typename T> struct nlohmann::adl_serializer<glm::vec<2, T>> {
  static void to_json(json &j, glm::vec<2, T> const &v) {
    j[0] = v.x;
    j[1] = v.y;
  }
  static void from_json(json const &j, glm::vec<2, T> &v) {
    DEBUG_ASSERT(j.is_array() && j.size() <= 2,
                 "2d vector must be an array of 2 floats");

    v.x = j.size() >= 1 ? j[0].get<T>() : static_cast<T>(0);
    v.y = j.size() >= 2 ? j[1].get<T>() : static_cast<T>(0);
  }
};

template <typename T> struct nlohmann::adl_serializer<glm::vec<3, T>> {
  static void to_json(json &j, glm::vec<3, T> const &v) {
    j[0] = v.x;
    j[1] = v.y;
    j[2] = v.z;
  }
  static void from_json(json const &j, glm::vec<3, T> &v) {
    DEBUG_ASSERT(j.is_array() && j.size() <= 3,
                 "3d vector must be an array of 3 floats");

    v.x = j.size() >= 1 ? j[0].get<T>() : static_cast<T>(0);
    v.y = j.size() >= 2 ? j[1].get<T>() : static_cast<T>(0);
    v.z = j.size() >= 3 ? j[2].get<T>() : static_cast<T>(0);
  }
};

template <typename T> struct nlohmann::adl_serializer<glm::vec<4, T>> {
  static void to_json(json &j, glm::vec<4, T> const &v) {
    j[0] = v.x;
    j[1] = v.y;
    j[2] = v.z;
    j[3] = v.w;
  }
  static void from_json(json const &j, glm::vec<4, T> &v) {
    DEBUG_ASSERT(j.is_array() && j.size() <= 4,
                 "4d vector must be an array of 4 floats");

    v.x = j.size() >= 1 ? j[0].get<T>() : static_cast<T>(0);
    v.y = j.size() >= 2 ? j[1].get<T>() : static_cast<T>(0);
    v.z = j.size() >= 3 ? j[2].get<T>() : static_cast<T>(0);
    v.w = j.size() >= 4 ? j[3].get<T>() : static_cast<T>(0);
  }
};

template <typename T> struct nlohmann::adl_serializer<glm::qua<T>> {
  static void to_json(json &j, glm::qua<T> const &v) {
    j[0] = v.x;
    j[1] = v.y;
    j[2] = v.z;
    j[3] = v.w;
  }
  static void from_json(json const &j, glm::qua<T> &v) {
    DEBUG_ASSERT(j.is_array() && j.size() <= 4,
                 "quaternion must be an array of 4 floats");

    v.x = j.size() >= 1 ? j[0].get<T>() : static_cast<T>(0);
    v.y = j.size() >= 2 ? j[1].get<T>() : static_cast<T>(0);
    v.z = j.size() >= 3 ? j[2].get<T>() : static_cast<T>(0);
    v.w = j.size() >= 4 ? j[3].get<T>() : static_cast<T>(0);
  }
};
