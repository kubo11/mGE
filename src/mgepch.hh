#ifndef MGE_MGEPCH_HH
#define MGE_MGEPCH_HH

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <exception>
#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

// glad
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

// glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// spdlog
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// mGE
#include "logger.hh"

#endif  // MGE_MGEPCH_HH