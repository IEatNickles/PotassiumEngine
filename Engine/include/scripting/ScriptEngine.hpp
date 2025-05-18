#pragma once

#include <filesystem>
#include <flecs.h>
#include <string>

namespace KEngine {
class ScriptEngine {
public:
  using AddComponentFn = void (*)(flecs::world &, std::string, flecs::entity_t);
  using DrawComponentsFn = void (*)(flecs::world &, flecs::entity_t);
  using DrawAddComponentFn = void (*)(flecs::world &, flecs::entity_t);
  using SaveComponentsFn = void (*)(flecs::world const &);
  using LoadComponentsFn = void (*)(flecs::world &);

  void init(std::filesystem::path const &script_dl);
  void add_component(flecs::world &reg, std::string name,
                     flecs::entity_t entity) {
    if (!m_add_component_fn)
      return;
    m_add_component_fn(reg, name, entity);
  }
  void draw_components(flecs::world &reg, flecs::entity_t entity) const {
    if (!m_draw_components_fn)
      return;
    m_draw_components_fn(reg, entity);
  }
  void draw_add_component(flecs::world &reg, flecs::entity_t entity) const {
    if (!m_draw_add_component_fn)
      return;
    m_draw_add_component_fn(reg, entity);
  }
  void save_components(flecs::world const &reg) const {
    if (!m_save_components_fn)
      return;
    m_save_components_fn(reg);
  }
  void load_components(flecs::world &reg) {
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
