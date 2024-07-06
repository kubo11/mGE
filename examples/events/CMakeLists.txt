file(GLOB_RECURSE EVENTS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(events ${EVENTS_SOURCES})

target_include_directories(events PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(events PRIVATE mge)