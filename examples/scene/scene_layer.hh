#ifndef MGE_SCENE_LAYER
#define MGE_SCENE_LAYER

#include "mge.hh"

struct CubeVertex {
  glm::vec3 position;
  glm::vec3 normal;

  inline static std::vector<mge::VertexAttribute> get_attributes() {
    return {{.size = 3u, .type = GL_FLOAT}, {.size = 3u, .type = GL_FLOAT}};
  }
};

class SceneLayer : public mge::Layer {
 public:
  SceneLayer(mge::Scene& scene, const mge::Timer& timer);
  ~SceneLayer() = default;

  virtual void configure() override;
  virtual void update() override;

  bool on_keyboard_key_updated(mge::KeyboardKeyUpdatedEvent& event);
  bool on_mouse_moved(mge::MouseMovedEvent& event);

 private:
  std::unique_ptr<mge::RenderPipeline<CubeVertex>> m_render_pipeline = nullptr;
  std::unique_ptr<mge::RenderableComponent<CubeVertex>> m_triangle = nullptr;
  mge::Scene& m_scene;
  const mge::Timer& m_timer;
  bool m_move_front = false;
  bool m_move_back = false;
  bool m_move_left = false;
  bool m_move_right = false;
  bool m_move_up = false;
  bool m_move_down = false;
  mge::EntityId m_cube_id;

  mge::Entity& create_cube(glm::vec3 position, float size);
};

#endif  // MGE_TRIANGLE_LAYER