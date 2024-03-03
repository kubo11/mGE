#include "render_context.hh"

namespace mge {
std::unique_ptr<RenderContext> RenderContext::s_instance = nullptr;

RenderContext::RenderContext() { MGE_INFO("Render context created"); }

RenderContext::~RenderContext() {}

RenderContext& RenderContext::create() {
  s_instance = std::unique_ptr<RenderContext>(new RenderContext());
  s_instance->init();
  return *s_instance;
}

void RenderContext::init() {
  if (!gladLoadGL(glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }

  MGE_INFO("Render context initialized");
}

void RenderContext::terminate() { MGE_INFO("Render context terminated"); }
}  // namespace mge
