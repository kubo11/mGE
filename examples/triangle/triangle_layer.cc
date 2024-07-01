#include "triangle_layer.hh"

TriangleLayer::TriangleLayer(mge::Scene& scene)
    : m_scene(scene), m_shader(mge::ShaderSystem::acquire(fs::current_path() / "shaders" / "triangle")) {}

void TriangleLayer::configure() {}
void TriangleLayer::update() {}