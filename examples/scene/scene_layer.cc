#include "scene_layer.hh"

#include <filesystem>
namespace fs = std::filesystem;

SceneLayer::SceneLayer(mge::Scene& scene, const mge::Timer& timer) : m_scene(scene), m_timer(timer) {}

void SceneLayer::configure() {
  mge::RenderPipelineBuilder pipeline_builder;
  m_render_pipeline = std::move(
      pipeline_builder
          .add_shader_program(
              mge::ShaderSystem::acquire(fs::current_path() / "examples" / "scene" / "shaders" / "phong"))
          .add_uniform_update<glm::mat4>(
              "viewMat", [&camera = m_scene.get_current_camera()]() { return camera.get_view_matrix(); })
          .add_uniform_update<glm::mat4>(
              "projMat", [&camera = m_scene.get_current_camera()]() { return camera.get_projection_matrix(); })
          .build<CubeVertex>(mge::DrawPrimitiveType::TRIANGLE));
  m_render_pipeline->dynamic_uniform_update<glm::vec3>("color", []() { return glm::vec3{1.0f, 0.0f, 0.0f}; });
  m_cube_id = create_cube({0.0f, 0.0f, 0.0f}, 1.0f).get_id();
  m_scene.get_entity(m_cube_id).add_child(create_cube({1.2f, 0.0f, 0.0f}, 0.5f));
  m_scene.get_entity(m_cube_id).add_child(create_cube({-1.2f, 0.0f, 0.0f}, 0.5f));
  mge::AddEventListener(mge::MouseEvents::MouseMoved, SceneLayer::on_mouse_moved, this);
  mge::AddEventListener(mge::KeyboardEvents::KeyboardKeyUpdated, SceneLayer::on_keyboard_key_updated, this);
}

void SceneLayer::update() {
  if (m_move_front) m_scene.get_current_camera().move(mge::Camera::MoveDirection::FRONT, m_timer.get_dt());
  if (m_move_back) m_scene.get_current_camera().move(mge::Camera::MoveDirection::BACK, m_timer.get_dt());
  if (m_move_left) m_scene.get_current_camera().move(mge::Camera::MoveDirection::LEFT, m_timer.get_dt());
  if (m_move_right) m_scene.get_current_camera().move(mge::Camera::MoveDirection::RIGHT, m_timer.get_dt());
  if (m_move_up) m_scene.get_current_camera().move(mge::Camera::MoveDirection::UP, m_timer.get_dt());
  if (m_move_down) m_scene.get_current_camera().move(mge::Camera::MoveDirection::DOWN, m_timer.get_dt());
  auto& cube = m_scene.get_entity(m_cube_id);
  cube.run_and_propagate([](auto& entity) {
    entity.template patch<mge::TransformComponent>([](auto& transform) {
      transform.rotate(0.0002f, {0.0f, 1.0f, 0.0f});
    });
  });
  m_render_pipeline->run();
}

mge::Entity& SceneLayer::create_cube(glm::vec3 position, float size) {
  auto& entity = m_scene.create_entity();
  entity.add_component<mge::TransformComponent>(position, glm::quat{1.0f, 0.0f, 0.0f, 0.0f},
                                                glm::vec3{size, size, size});
  static auto vertices = std::vector<CubeVertex>{// back
                                                 {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                                                 {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                                                 {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                                                 {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                                                 {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                                                 {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                                                 // front
                                                 {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                 {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                 {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                 {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                 {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                 {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                 // left
                                                 {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
                                                 {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
                                                 {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
                                                 {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
                                                 {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
                                                 {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
                                                 // right
                                                 {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
                                                 {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                 {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                 {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                 {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
                                                 {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
                                                 // down
                                                 {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
                                                 {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
                                                 {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
                                                 {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
                                                 {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
                                                 {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
                                                 // up
                                                 {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                 {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                                 {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                 {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                                 {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                 {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}};
  auto vertex_buffer = std::make_unique<mge::Buffer<CubeVertex>>();
  vertex_buffer->bind();
  vertex_buffer->copy(vertices);
  vertex_buffer->unbind();
  auto vertex_array =
      std::make_unique<mge::VertexArray<CubeVertex>>(std::move(vertex_buffer), CubeVertex::get_attributes());
  entity.add_component<mge::RenderableComponent<CubeVertex>>(
      mge::RenderPipelineMap<CubeVertex>{{mge::RenderMode::SOLID, *m_render_pipeline}}, mge::RenderMode::SOLID,
      std::move(vertex_array), [&entity](mge::RenderPipeline<CubeVertex>& render_pipeline) {
        render_pipeline.dynamic_uniform_update_and_commit<glm::mat4>(
            "modelMat", [&entity]() { return entity.get_component<mge::TransformComponent>().get_model_mat(); });
      });
  return entity;
}

bool SceneLayer::on_keyboard_key_updated(mge::KeyboardKeyUpdatedEvent& event) {
  switch (event.key) {
    case mge::KeyboardKey::W:
      if (event.state == mge::InputState::Press) m_move_front = true;
      if (event.state == mge::InputState::Release) m_move_front = false;
      break;
    case mge::KeyboardKey::S:
      if (event.state == mge::InputState::Press) m_move_back = true;
      if (event.state == mge::InputState::Release) m_move_back = false;
      break;
    case mge::KeyboardKey::A:
      if (event.state == mge::InputState::Press) m_move_left = true;
      if (event.state == mge::InputState::Release) m_move_left = false;
      break;
    case mge::KeyboardKey::D:
      if (event.state == mge::InputState::Press) m_move_right = true;
      if (event.state == mge::InputState::Release) m_move_right = false;
      break;
    case mge::KeyboardKey::Space:
      if (event.state == mge::InputState::Press) m_move_up = true;
      if (event.state == mge::InputState::Release) m_move_up = false;
      break;
    case mge::KeyboardKey::LeftShift:
      if (event.state == mge::InputState::Press) m_move_down = true;
      if (event.state == mge::InputState::Release) m_move_down = false;
      break;
  }
  return true;
}

bool SceneLayer::on_mouse_moved(mge::MouseMovedEvent& event) {
  if (!event.source_window.is_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) return false;
  m_scene.get_current_camera().rotate(event.end_position.x - event.start_position.x,
                                      event.end_position.y - event.start_position.y, event.get_dt());
  return true;
}