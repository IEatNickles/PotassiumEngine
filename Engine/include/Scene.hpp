#pragma once

#include <cstdlib>
#include <memory>
#include <string>
#include <tuple>

#include "Renderer.hpp"
#include "UUID.hpp"
#include "Version.hpp"
#include "entt/entt.hpp"
#include "nlohmann/json_fwd.hpp"
#include "scripting/ScriptEngine.hpp"

namespace KEngine {
enum class SystemType { Start, Update, End };

class SystemRegistry {
public:
  template <typename F> void add(SystemType type, F &&fn) {
    using tys = typename function_traits<F>::args;
    m_fns[(int)type].push_back([&]() {
      [&]<typename... Ts>(std::tuple<Ts...> *) {
        m_ecs->view<Ts...>().each(fn);
      }(static_cast<tys *>(nullptr));
    });
  }

private:
  std::array<std::vector<std::function<void()>>, (int)SystemType::End> m_fns;
  std::shared_ptr<entt::registry> m_ecs;

  template <typename F> struct function_traits;

  template <typename R, typename... Args>
  struct function_traits<R (*)(Args...)> {
    using args = std::tuple<Args...>;
    using return_type = R;
  };

  template <typename R, typename... Args>
  struct function_traits<std::function<R (*)(Args...)>> {
    using args = std::tuple<Args...>;
    using return_type = R;
  };

  template <typename N, typename R, typename... Args>
  struct function_traits<R (N::*)(Args...)> {
    using args = std::tuple<Args...>;
    using return_type = R;
  };

  template <typename N, typename R, typename... Args>
  struct function_traits<R (N::*)(Args...) const> {
    using args = std::tuple<Args...>;
    using return_type = R;
  };

  template <typename F>
  struct function_traits : function_traits<decltype(&F::operator())> {};

  friend class Scene;
};

class Scene final {
public:
  Scene();

  void start();
  void update();
  void end();

  void save();
  void save(std::filesystem::path const &path);
  void load(std::filesystem::path const &path);

  void create(std::string const &name);

  template <typename T> bool has(entt::entity e) { return m_ecs.all_of<T>(e); }

  template <typename T> T &get(entt::entity e) { return m_ecs.get<T>(e); }

  template <typename T> T &get_or_add(entt::entity e) {
    return m_ecs.get_or_emplace<T>(e);
  }

  template <typename T> T *try_get(entt::entity e) {
    return m_ecs.try_get<T>(e);
  }

  template <typename T, typename... Ts> T &set(entt::entity e, Ts... args) {
    return m_ecs.emplace<T>(e, args...);
  }

  template <typename T> bool remove_component(entt::entity e) {
    return m_ecs.remove<T>(e);
  }

  bool is_valid_entity(entt::entity e) { return m_ecs.valid(e); }

  KEngine::ScriptEngine const &script_engine() const { return m_script_engine; }

  entt::entity entity() { return m_ecs.create(); }
  entt::entity entity(UUID uuid) { return m_ecs.create(); }

  entt::registry &ecs() { return m_ecs; }

  std::string const &name() const { return m_name; }

  void version_0_0(nlohmann::json &es);
  void resolve_version(Version version, nlohmann::json &es);

  std::filesystem::path const &path() const { return m_path; }

private:
  std::string m_name;
  std::filesystem::path m_path;
  Version m_version;

  Renderer m_renderer;

  SystemRegistry m_sys_registry;

  KEngine::ScriptEngine m_script_engine;

  entt::registry m_ecs;

  std::function<void(entt::registry const &)> m_extern_component_save;
  std::function<void(entt::registry &, nlohmann::json const &)>
      m_extern_component_load;
};
} // namespace KEngine
