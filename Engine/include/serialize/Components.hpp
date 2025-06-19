#pragma once

#include "components/Camera.hpp"
#include "components/MeshRenderer.hpp"
#include "components/Name.hpp"
#include "components/Relationship.hpp"
#include "components/Transform.hpp"

#include "serialize/glm.hpp"

#include "nlohmann/json.hpp"

template <> struct nlohmann::adl_serializer<MeshRendererComponent> {
  static void to_json(nlohmann::json &j, MeshRendererComponent const &c) {
    j["mesh"] = "This is a mesh, do not change lest bad things (nothing at "
                "all) will happen, thank you";
  }

  static void from_json(nlohmann::json const &j, MeshRendererComponent &c) {
    c.mesh =
        Mesh({{-0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}},
             {0, 1, 2});
  }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RelationshipComponent, child_count, first,
                                   last, prev, next, parent);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NameComponent, name);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraComponent, projection, size, aspect,
                                   z_near, z_far, clear_color);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransformComponent, position, rotation,
                                   scale);
