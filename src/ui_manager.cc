#include "ui_manager.hh"

namespace mge {
std::shared_ptr<UIManager> UIManager::s_instance = nullptr;

UIManager::UIManager(Window& window) {
  MGE_INFO("UI manager created");
  s_instance->init(window);
}

UIManager::~UIManager() {
  if (s_instance) {
    terminate();
  }
}

std::shared_ptr<UIManager> UIManager::create(Window& window) {
  if (s_instance) {
    throw std::runtime_error("UI manager already exists!");
  }

  s_instance = std::shared_ptr<UIManager>(new UIManager(window));
  return s_instance;
}

void UIManager::start_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void UIManager::end_frame() { ImGui::EndFrame(); }

void UIManager::init(Window& window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.get_instance(), true);
  ImGui_ImplOpenGL3_Init("#version 460 core");

  MGE_INFO("UI manager initialized");
}

void UIManager::terminate() {
  s_instance = nullptr;

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  MGE_INFO("UI manager terminated");
}
}  // namespace mge
