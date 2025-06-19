#pragma once

#include <cstdint>

#include "Log.hpp"
#include "nlohmann/json.hpp"

struct Version {
  Version() : minor(0), major(0) {}
  Version(uint32_t minor, uint32_t major) : minor(minor), major(major) {}

  uint32_t minor, major;
};

static bool operator==(Version const &a, Version const &b) {
  return a.minor == b.minor && a.major == b.major;
}

inline void to_json(nlohmann::json &j, Version const &v) {
  j = nlohmann::json::array({v.major, v.minor});
}

inline void from_json(nlohmann::json const &j, Version &v) {
  DEBUG_ASSERT(j.size() == 2, "Version must have a minor and major");
  v.major = j[0];
  v.minor = j[1];
}
