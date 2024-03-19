file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/mge)

target_precompile_headers(${PROJECT_NAME} PUBLIC mgepch.hh)

target_link_libraries(${PROJECT_NAME} PUBLIC EnTT::EnTT glad glfw glm imgui spdlog)