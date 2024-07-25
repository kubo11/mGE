#include "scene_application.hh"

int main() {
  auto app = SceneApplication();
  app.run();
  app.terminate();
  return 0;
}