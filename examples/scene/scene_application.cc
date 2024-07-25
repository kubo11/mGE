#include "scene_application.hh"
#include "scene_layer.hh"

SceneApplication::SceneApplication() { push_layer(std::move(std::make_unique<SceneLayer>(*m_scene, *m_timer))); }