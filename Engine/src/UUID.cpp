#include "UUID.hpp"
#include <random>

static std::random_device device;
static std::mt19937_64 engine(device());
static std::uniform_int_distribution<uint64_t> distrib;

UUID::UUID() { m_uuid = distrib(engine); }
