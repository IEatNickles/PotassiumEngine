#include "Core.hpp"
#include "Scene.hpp"

KCOMPONENT()
struct Foo {
  int foo;
  float bar;
};

#define SYSTEM(name)                                                           \
  class name##System {                                                         \
  public:                                                                      \
    void run(entt::registry reg);                                              \
  };                                                                           \
                                                                               \
  inline void name##System::run(entt::registry reg)

SYSTEM(Fart) {
  reg.view<int>().each([]() {});
}
