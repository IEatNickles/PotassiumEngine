#pragma once

#include <cstdint>

struct UUID {
public:
  UUID();
  UUID(uint64_t uuid) : m_uuid(uuid) {}
  UUID(UUID const &) = default;
  UUID(UUID &&) = default;

  operator uint64_t() const noexcept { return m_uuid; }
  UUID &operator=(UUID const &) = default;

private:
  uint64_t m_uuid;
};

namespace std {
template <typename T> struct hash;

template <> struct hash<UUID> {
  size_t operator()(UUID const &id) const noexcept { return (size_t)id; }
};
} // namespace std
