file(GLOB_RECURSE SCENE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(scene ${SCENE_SOURCES})

target_include_directories(scene PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(scene PRIVATE mge)