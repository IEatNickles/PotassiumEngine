#pragma once

#include <filesystem>
#include <string>

#include "entt/entt.hpp"

namespace KEngine {
class ScriptEngine {
public:
  using AddComponentFn = void (*)(entt::registry &, std::string, entt::entity);
  using DrawComponentsFn = void (*)(entt::registry &, entt::entity);
  using DrawAddComponentFn = void (*)(entt::registry &, entt::entity);
  using SaveComponentsFn = void (*)(entt::registry const &);
  using LoadComponentsFn = void (*)(entt::registry &);

  void init(std::filesystem::path const &script_dl);
  void add_component(entt::registry &reg, std::string name,
                     entt::entity entity) {
    if (!m_add_component_fn)
      return;
    m_add_component_fn(reg, name, entity);
  }
  void draw_components(entt::registry &reg, entt::entity entity) const {
    if (!m_draw_components_fn)
      return;
    m_draw_components_fn(reg, entity);
  }
  void draw_add_component(entt::registry &reg, entt::entity entity) const {
    if (!m_draw_add_component_fn)
      return;
    m_draw_add_component_fn(reg, entity);
  }
  void save_components(entt::registry const &reg) const {
    if (!m_save_components_fn)
      return;
    m_save_components_fn(reg);
  }
  void load_components(entt::registry &reg) {
    if (!m_load_components_fn)
      return;
    m_load_components_fn(reg);
  }

private:
  AddComponentFn m_add_component_fn = nullptr;
  DrawComponentsFn m_draw_components_fn = nullptr;
  DrawAddComponentFn m_draw_add_component_fn = nullptr;
  SaveComponentsFn m_save_components_fn = nullptr;
  LoadComponentsFn m_load_components_fn = nullptr;
};
} // namespace KEngine
