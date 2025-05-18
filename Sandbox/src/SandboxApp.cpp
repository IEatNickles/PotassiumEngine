#include "Scene.hpp"
#include <Application.hpp>
#include <Renderer.hpp>
#include <Window.hpp>

namespace KEngine {
class SandboxApp : public App {
public:
  virtual void run() override {
    Window win(1280, 720, "Window");
    Scene scene;

    scene.start();
    while (!win.should_close()) {
      scene.update();

      win.new_frame();
    }
    scene.end();
  }
};

App *create_app() { return new SandboxApp(); }
} // namespace KEngine
