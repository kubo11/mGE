# glad
add_subdirectory(glad/cmake)
glad_add_library(glad REPRODUCIBLE API gl:core=4.6)

# glfw
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

# spdlog
add_subdirectory(spdlog)