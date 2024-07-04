file(GLOB_RECURSE TRIANGLE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(triangle ${TRIANGLE_SOURCES})

target_include_directories(triangle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(triangle PRIVATE mge)