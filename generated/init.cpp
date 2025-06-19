#include <cstdint>
#include <string>
#include <type_traits>

#include "entt/entt.hpp"
#include "imgui.h"
#include "nlohmann/json.hpp"

#include "Sandbox/assets/Foo.hpp-generated.hpp"

extern "C" void save(nlohmann::json &j, entt::registry const &reg) {
  FooReflected::save(j, reg);
}
extern "C" void load(nlohmann::json const &j, entt::registry &reg) {
  if (j.contains("Foo")) {
    FooReflected::load(j["Foo"], reg);
  }
}

extern "C" void add_component(entt::registry &reg, std::string const &name,
                              entt::entity e) {
  if (name == "Foo") {
    reg.emplace<Foo>(e);
    return;
  }
}
extern "C" void draw_components(entt::registry &reg, entt::entity e) {
  if (reg.all_of<Foo>(e)) {
    if (ImGui::CollapsingHeader("Foo")) {
      auto &c = reg.get<Foo>(e);
      if constexpr (std::is_same<decltype(Foo::foo), int>()) {
        ImGui::DragInt("foo", (int *)&c.foo);
      } else if constexpr (std::is_same<decltype(Foo::foo), float>()) {
        ImGui::DragFloat("foo", (float *)&c.foo);
      }
      if constexpr (std::is_same<decltype(Foo::bar), int>()) {
        ImGui::DragInt("bar", (int *)&c.bar);
      } else if constexpr (std::is_same<decltype(Foo::bar), float>()) {
        ImGui::DragFloat("bar", (float *)&c.bar);
      }
    }
  }
}
extern "C" void draw_add_component(entt::registry &reg, entt::entity e) {
  if (!reg.all_of<Foo>(e)) {
    if (ImGui::MenuItem("Foo")) {
      reg.emplace<Foo>(e);
    }
  }
}
