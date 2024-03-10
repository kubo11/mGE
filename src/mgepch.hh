#ifndef MGE_MGEPCH_HH
#define MGE_MGEPCH_HH

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <functional>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

// glad
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

// glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/gtc/matrix_transform.hpp"

// imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// spdlog
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// mGE
#include "logger.hh"

#endif  // MGE_MGEPCH_HH