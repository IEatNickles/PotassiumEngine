#include "Version.hpp"
#define JSON_NOEXCEPTION

#include "Scene.hpp"

#include <cstring>
#include <filesystem>

#include <fstream>

#include <cmath>
#include <string>

#include "glad/glad.h"

#include "Core.hpp"

#include "Log.hpp"
#include "Viewport.hpp"

#include "components/Camera.hpp"
#include "components/MeshRenderer.hpp"
#include "components/Name.hpp"
#include "components/Relationship.hpp"
#include "components/Transform.hpp"

#include "serialize/Components.hpp"

// #include "scripting/ScriptEngine.hpp"

namespace KEngine {
Scene::Scene() {
  create("New Scene");
  m_renderer.init();
}

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

#define SERIALIZE_COMPONENTS(type, _json)                                      \
  {                                                                            \
    auto cs = nlohmann::json::array();                                         \
    m_ecs.view<type>().each([&cs](entt::entity e, type const &c) {             \
      cs.push_back(                                                            \
          nlohmann::json::object({{"entity", (uint64_t)e}, {"data", c}}));     \
    });                                                                        \
    _json[#type] = cs;                                                         \
  }

void Scene::save() {
  if (m_path.empty()) {
    DEBUG_ERROR("Could not save scene, there is no associated file");
    return;
  }

  save(m_path);
}

void Scene::save(std::filesystem::path const &path) {
  nlohmann::json j;
  j["version"] = m_version;

  auto es = nlohmann::json::object();
  SERIALIZE_COMPONENTS(TransformComponent, es);
  SERIALIZE_COMPONENTS(CameraComponent, es);
  SERIALIZE_COMPONENTS(RelationshipComponent, es);
  SERIALIZE_COMPONENTS(NameComponent, es);
  SERIALIZE_COMPONENTS(MeshRendererComponent, es);

  j["entities"] = es;

  // m_script_engine.save_components(m_ecs);

  std::ofstream ofs(path);
  ofs << j.dump(2) << "\n";
  ofs.close();
}

#define DESERIALIZE_COMPONENTS(type, _json)                                    \
  if (_json.contains(#type)) {                                                 \
    auto cs = _json[#type];                                                    \
    for (auto it : cs) {                                                       \
      auto e = (entt::entity)it["entity"].get<uint64_t>();                     \
      if (!m_ecs.valid(e)) {                                                   \
        e = m_ecs.create(e);                                                   \
      }                                                                        \
      m_ecs.emplace<type>(e, it["data"].get<type>());                          \
    }                                                                          \
  }

void Scene::load(std::filesystem::path const &path) {
  std::ifstream ifs(path);
  nlohmann::json j;
  ifs >> j;

  Version file_version = m_version = K_VERSION;
  m_path = path;
  m_name = m_path.filename().stem();

  if (j.contains("version")) {
    file_version = j["version"].get<Version>();
  }

  if (j.contains("entities")) {
    auto es = j["entities"];
    resolve_version(file_version, es);
  }

  ifs.close();

  m_renderer.init();

  m_ecs.on_construct<CameraComponent>()
      .connect<&entt::registry::get_or_emplace<TransformComponent>>();
  m_ecs.on_construct<MeshRendererComponent>()
      .connect<&entt::registry::get_or_emplace<TransformComponent>>();
}

void Scene::version_0_0(nlohmann::json &es) {
  DESERIALIZE_COMPONENTS(TransformComponent, es);
  DESERIALIZE_COMPONENTS(CameraComponent, es);
  DESERIALIZE_COMPONENTS(RelationshipComponent, es);
  DESERIALIZE_COMPONENTS(NameComponent, es);
  DESERIALIZE_COMPONENTS(MeshRendererComponent, es);
}

void Scene::resolve_version(Version version, nlohmann::json &es) {
  if (version == Version(0, 0)) {
    version_0_0(es);
  }
}

void Scene::create(std::string const &name) {
  m_name = name;
  m_version = Version(K_VERSION_MINOR, K_VERSION_MAJOR);

  m_ecs.on_construct<CameraComponent>()
      .connect<&entt::registry::get_or_emplace<TransformComponent>>();
  m_ecs.on_construct<MeshRendererComponent>()
      .connect<&entt::registry::get_or_emplace<TransformComponent>>();
}
} // namespace KEngine
