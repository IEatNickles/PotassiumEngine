#include "components/Camera.hpp"
#include "components/MeshRenderer.hpp"
#include "components/Transform.hpp"

#include "serialize/glm.hpp"
#include <nlohmann/json.hpp>

template <> struct nlohmann::adl_serializer<TransformComponent> {
  static void to_json(nlohmann::json &j, TransformComponent const &c) {
    j["position"] = c.position;
    j["rotation"] = c.rotation;
    j["scale"] = c.scale;
  }

  static void from_json(nlohmann::json const &j, TransformComponent &c) {
    if (j.contains("position")) {
      c.position = j["position"].get<glm::vec3>();
    }
    if (j.contains("rotation")) {
      c.rotation = j["rotation"].get<glm::quat>();
    }
    if (j.contains("scale")) {
      c.scale = j["scale"].get<glm::vec3>();
    }
  }
};

template <> struct nlohmann::adl_serializer<CameraComponent> {
  static void to_json(nlohmann::json &j, CameraComponent const &c) {
    j["size"] = c.size;
    j["projection"] = c.projection;
    j["aspect"] = c.aspect;
    j["z_near"] = c.z_near;
    j["z_far"] = c.z_far;
  }

  static void from_json(nlohmann::json const &j, CameraComponent &c) {
    if (j.contains("size")) {
      c.size = j["size"].get<float>();
    }
    if (j.contains("projection")) {
      c.projection = (ProjectionType)j["projection"].get<int>();
    }
    if (j.contains("aspect")) {
      c.aspect = j["aspect"].get<float>();
    }
    if (j.contains("z_near")) {
      c.z_near = j["z_near"].get<float>();
    }
    if (j.contains("z_far")) {
      c.z_far = j["z_far"].get<float>();
    }
  }
};

template <> struct nlohmann::adl_serializer<MeshRendererComponent> {
  static void to_json(nlohmann::json &j, MeshRendererComponent const &c) {}

  static void from_json(nlohmann::json const &j, MeshRendererComponent &c) {
    c.mesh =
        Mesh({{-0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}},
             {0, 1, 2});
  }
};
