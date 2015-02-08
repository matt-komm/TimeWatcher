project(timewatcher)
cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_CXX_FLAGS "-Wextra -Wall -pedantic -std=c++0x -Wno-unused-parameter"${CMAKE_CXX_FLAGS} )
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/release)


add_executable(test test.cpp)
target_link_libraries(test pthread)