file(GLOB_RECURSE INSTANCING_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(instancing ${INSTANCING_SOURCES})

target_include_directories(instancing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(instancing PRIVATE mge)