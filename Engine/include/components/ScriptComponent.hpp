#pragma once

#include <functional>

struct ScriptComponent {
  std::function<void()> start;
  std::function<void(float)> update;
};
