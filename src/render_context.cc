#include "render_context.hh"

namespace mge {
std::unique_ptr<RenderContext> RenderContext::s_instance = nullptr;

RenderContext::RenderContext() {
  MGE_INFO("Render context created");
  s_instance->init();
}

RenderContext::~RenderContext() {
  if (s_instance) {
    s_instance->terminate();
  }
}

RenderContext& RenderContext::create() {
  if (s_instance) {
    throw std::runtime_error("Render context already exists!");
  }

  s_instance = std::unique_ptr<RenderContext>(new RenderContext());
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
