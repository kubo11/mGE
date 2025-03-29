# entt
add_subdirectory(entt)

# glad
set(OPENGL_EXTENSIONS GL_ARB_shader_storage_buffer_object)
add_subdirectory(glad/cmake)
glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6 EXTENSIONS ${OPENGL_EXTENSIONS})

# glfw
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

# glm
add_subdirectory(glm)

# imgui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/*.h)
add_library(
    imgui
    ${IMGUI_SOURCES}
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.h
)

target_link_libraries(imgui PUBLIC glad)
target_link_libraries(imgui PUBLIC glfw)
target_include_directories(
    imgui
    PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

#imguizmo
set(IMGUIZMO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo)
set(IMGUIZMO_SOURCES ${IMGUIZMO_DIR}/ImGuizmo.cpp ${IMGUIZMO_DIR}/ImGuizmo.h)
add_library(imguizmo ${IMGUIZMO_SOURCES})
target_link_libraries(imguizmo PUBLIC imgui)
target_include_directories(imguizmo PUBLIC ${IMGUIZMO_DIR})
target_compile_definitions(imguizmo PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

# spdlog
add_subdirectory(spdlog)