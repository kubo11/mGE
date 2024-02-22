file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

#add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_precompile_headers(${PROJECT_NAME} PRIVATE mgepch.hh)

target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw spdlog)

add_custom_target(create_exec_link ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} ${BUILD_ROOT}/${PROJECT_NAME})

add_dependencies(create_exec_link ${PROJECT_NAME})