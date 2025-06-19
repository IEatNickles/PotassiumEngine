
#include <vector>
#include <cstdint>

#include "nlohmann/json.hpp"
#include "entt/entt.hpp"

#include "Sandbox/assets/Foo.hpp"

using namespace entt::literals;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Foo, foo, bar);

namespace FooReflected {
inline std::vector<entt::id_type> prop_ids = { "foo"_hs, "bar"_hs };

inline std::map<entt::id_type, char const *> id_to_name = {
  {entt::type_id<Foo>().index(), "Foo"},
  {prop_ids[0], "foo"}, {prop_ids[1], "bar"}
};

inline void save(nlohmann::json &j, entt::registry const &reg) {
  nlohmann::json cs = nlohmann::json::array();
  reg.view<Foo>().each([&cs](entt::entity e, Foo const &c) {
    cs.push_back({ { "entity", (uint64_t)e }, { "data", c } });
  });
  j["Foo"] = cs;
}

inline void load(nlohmann::json const &j, entt::registry &reg) {
  for (auto val : j) {
    entt::entity e = (entt::entity)val["entity"].get<uint64_t>();
    Foo c = j["data"].get<Foo>();
    reg.emplace<Foo>(e, c);
  }
}
} // namespace FooReflected
