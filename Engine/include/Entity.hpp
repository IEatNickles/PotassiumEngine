#pragma once

#include "Scene.hpp"

namespace KEngine {
class Entity {
public:
private:
  Entity(Scene *scene, entt::entity id);

  entt::entity m_id;
  Scene *m_scene;

  friend Scene;
};
} // namespace KEngine
