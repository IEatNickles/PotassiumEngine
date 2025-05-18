#pragma once

#include "Core.hpp"

namespace KEngine {
class K_API App {
public:
  virtual ~App() = default;
  virtual void run() = 0;
};

App *create_app();
} // namespace KEngine
