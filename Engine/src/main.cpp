#include "Application.hpp"

extern KEngine::App *KEngine::create_app();

int main() {
  KEngine::App *app = KEngine::create_app();
  app->run();
}
