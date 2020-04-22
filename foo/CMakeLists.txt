project(foo)

add_library(foo SHARED foo.cpp new.cpp)

target_include_directories(foo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_options(foo PRIVATE "-Wl,--version-script=${CMAKE_CURRENT_LIST_DIR}/libfoo.version")
