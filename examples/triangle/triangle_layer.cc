#include "triangle_layer.hh"

#include <filesystem>
namespace fs = std::filesystem;

TriangleLayer::TriangleLayer()
    : m_shader(mge::ShaderSystem::acquire(fs::current_path() / "examples" / "triangle" / "shaders" / "triangle")),
      m_vertex_buffer(std::make_unique<mge::Buffer<TriangleVertex>>()),
      m_vertex_array(std::make_unique<mge::VertexArray>()) {}

void TriangleLayer::configure() {
    auto vertices = std::vector<TriangleVertex>{
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};
    m_vertex_buffer->bind();
    m_vertex_buffer->copy(vertices);
    m_vertex_buffer->unbind();
    m_vertex_array->bind();
    m_vertex_array->attach_buffer(*m_vertex_buffer.get(), TriangleVertex::get_attributes());
    m_vertex_array->unbind();
}

void TriangleLayer::update() {
    m_shader->bind();
    m_vertex_array->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_vertex_buffer->get_count());
    m_vertex_array->unbind();
    m_shader->unbind();
}