#include "Core.hpp"

#include <string>
#include <typeindex>
#include <unordered_map>

#include "entt/entt.hpp"
using namespace entt::literals;
#include "nlohmann/json.hpp"

KCOMPONENT()
struct Foo {
  int foo;
  float bar;
};
// NLOHMANN_DEFINE_TYPE_INTRUSIVE();

struct FieldData {
  char const *name;
  std::type_index type;
};

#define EXPAND(x) x
#define GET_MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME
#define PASTE(...)                                                             \
  EXPAND(GET_MACRO(__VA_ARGS__, PASTE5, PASTE4, PASTE3, PASTE2,                \
                   PASTE1)(__VA_ARGS__))
#define PASTE2(fn, v1) fn(v1)
#define PASTE3(fn, v1, v2) PASTE2(fn, v1) PASTE2(fn, v2)
#define PASTE4(fn, v1, v2, v3) PASTE2(fn, v1) PASTE3(fn, v2, v3)
#define PASTE5(fn, v1, v2, v3, v4) PASTE2(fn, v1) PASTE4(fn, v2, v3, v4)

#define ENTT_HASH_STRING(x) #x##_hs,
#define KCMP(name, ...)                                                        \
  struct Register##name {                                                      \
    std::vector<entt::id_type> prop_hashes = {                                 \
        PASTE(ENTT_HASH_STRING, __VA_ARGS__)};                                 \
  };

KCMP(Foo, foo, bar);

// KCMP(Fart, FIELD(int, pee) FIELD(float, poop));

std::unordered_map<std::string, int> m_mape = {};
