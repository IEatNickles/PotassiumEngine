#include "Scene.hpp"

#include <cstring>
#include <filesystem>
#include <glad/glad.h>

#include <fstream>

#include <GLFW/glfw3.h>

#include <cmath>
#include <string>

#include "Log.hpp"
#include "Viewport.hpp"

#include "components/Camera.hpp"
#include "components/MeshRenderer.hpp"
#include "components/Transform.hpp"

#include "nlohmann/json_fwd.hpp"
// #include "scripting/ScriptEngine.hpp"

#include <nlohmann/json.hpp>

#include "serialize/Components.hpp"

namespace KEngine {
Scene::Scene() { create("New Scene"); }

void Scene::start() {
  for (auto fn : m_sys_registry.m_fns[(int)SystemType::Start]) {
    fn();
  }
}

void Scene::update() {
  m_ecs.view<CameraComponent, TransformComponent>().each(
      [this](CameraComponent &c, TransformComponent &t) {
        glClearColor(c.clear_color.r, c.clear_color.g, c.clear_color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_renderer.set_viewport(Viewport{c.projection, t.position, t.rotation,
                                         c.size, c.aspect, c.z_near, c.z_far});
        m_ecs.view<MeshRendererComponent, TransformComponent>().each(
            [this](MeshRendererComponent &m, TransformComponent &t) {
              m_renderer.draw_mesh(m.mesh, t);
            });
      });

  for (auto fn : m_sys_registry.m_fns[(int)SystemType::Update]) {
    fn();
  }
}

void Scene::end() {
  for (auto fn : m_sys_registry.m_fns[(int)SystemType::End]) {
    fn();
  }
}

struct EnTTJsonArchiveOut {
  EnTTJsonArchiveOut() { m_root = nlohmann::json::array(); }

  void operator()(std::underlying_type_t<entt::entity> size) {
    if (!m_curr.empty()) {
      m_root.push_back(m_curr);
    }
    m_curr = nlohmann::json::array();
    m_curr.push_back(size);
  }

  void operator()(entt::entity e) { m_curr.push_back((uint64_t)e); }

  template <typename T> void operator()(T const &c) { m_curr.push_back(c); }

  void output(std::filesystem::path const &path) {
    if (!m_curr.empty()) {
      m_root.push_back(m_curr);
    }

    std::ofstream ofs(path);

    ofs << m_root.dump(2);

    ofs.close();
  }

  std::string dump(const int indent = -1, const char indent_char = ' ',
                   const bool ensure_ascii = false,
                   const nlohmann::detail::error_handler_t error_handler =
                       nlohmann::detail::error_handler_t::strict) {
    return m_root.dump(indent, indent_char, ensure_ascii, error_handler);
  }

  nlohmann::json root() { return m_root; }

private:
  nlohmann::json m_root;
  nlohmann::json m_curr;
};

struct EnTTJsonArchiveIn {
  EnTTJsonArchiveIn(std::string const &json) {
    m_root = nlohmann::json::parse(json);
  }

  EnTTJsonArchiveIn(nlohmann::json const &json) { m_root = json; }

  void next_root() {
    ++m_root_idx;
    if (m_root_idx >= m_root.size()) {
      return;
    }

    m_curr = m_root[m_root_idx];
    m_curr_idx = 0;
  }

  void operator()(std::underlying_type_t<entt::entity> &size) {
    next_root();
    int s = m_curr[0].get<int>();
    ++m_curr_idx;
    size = (std::underlying_type_t<entt::entity>)s;
  }

  void operator()(entt::entity &e) {
    uint64_t id = m_curr[m_curr_idx].get<uint64_t>();
    e = (entt::entity)id;
    ++m_curr_idx;
  }

  template <typename T> void operator()(T &c) {
    c = m_curr[m_curr_idx].get<T>();
    ++m_curr_idx;
  }

private:
  nlohmann::json m_root;
  nlohmann::json m_curr;

  int m_root_idx = -1;
  int m_curr_idx = 0;
};

void Scene::save(std::filesystem::path const &path) {
  unsigned short version = K_VERSION;

  nlohmann::json j;
  j["version"] = version;

  EnTTJsonArchiveOut out_arch;
  entt::snapshot{m_ecs}.get<TransformComponent>(out_arch).get<CameraComponent>(
      out_arch);

  j["entities"] = out_arch.root();

  // m_script_engine.save_components(m_ecs);

  std::ofstream ofs(path);
  ofs << j.dump(2) << "\n";
  ofs.close();
}

void Scene::load(std::filesystem::path const &path) {
  std::ifstream ifs(path);
  nlohmann::json j;
  ifs >> j;

  if (!j.contains("version")) {
    DEBUG_ERROR("The scene does not have a version");
    return;
  }

  if (!j.contains("world")) {
    DEBUG_ERROR("The scene does not have a world");
    return;
  }

  m_name = path.filename().stem();
  m_version = j["version"].get<uint16_t>();

  EnTTJsonArchiveIn in_arch(j["entities"]);
  entt::snapshot_loader{m_ecs}
      .get<TransformComponent>(in_arch)
      .get<CameraComponent>(in_arch);

  ifs.close();

  m_renderer.init();

  m_ecs.on_construct<CameraComponent>()
      .connect<[](entt::registry &reg, entt::entity e) {
        reg.emplace<TransformComponent>(e);
      }>();
}

void Scene::create(std::string const &name) {
  m_name = name;
  m_version = K_VERSION;
}
} // namespace KEngine
