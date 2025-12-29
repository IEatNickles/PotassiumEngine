#include "Entity.hpp"

namespace KEngine {
Entity::Entity(Scene *scene, entt::entity id) : m_scene(scene), m_id(id) {}
} // namespace KEngine
