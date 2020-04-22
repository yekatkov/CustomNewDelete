cmake_minimum_required(VERSION 3.5)

project(CustomNewDelete LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_BUILD_TYPE Release)

set(USE_INLINE TRUE CACHE BOOL "")

if(USE_INLINE)
	# FAIL: We will catch SIGABRT
	set(CMAKE_CXX_FLAGS_RELEASE "-O1 -g -ggdb -DNDEBUG ")
else()
	# OK: with -fno-inline we don't catch SIGABRT with -O1 optimization
	set(CMAKE_CXX_FLAGS_RELEASE "-O1 -g -ggdb -fno-inline -DNDEBUG ")
endif()


add_subdirectory(foo)

add_executable(CustomNewDelete main.cpp)
target_link_libraries(CustomNewDelete PRIVATE foo)
