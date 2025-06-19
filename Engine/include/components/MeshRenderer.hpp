#pragma once

#include "Renderer.hpp"

struct MeshRendererComponent {
  MeshRendererComponent() = default;
  explicit MeshRendererComponent(Mesh mesh) : mesh(mesh) {}
  Mesh mesh =
      Mesh({{-0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}},
           {0, 1, 2});
};
