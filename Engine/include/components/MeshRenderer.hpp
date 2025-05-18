#pragma once

#include "Renderer.hpp"

struct MeshRendererComponent {
  MeshRendererComponent() = default;
  explicit MeshRendererComponent(Mesh mesh) : mesh(mesh) {}
  Mesh mesh;
};
