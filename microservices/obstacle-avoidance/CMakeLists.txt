cmake_minimum_required(VERSION 3.0.2)
project(obstacle-avoidance)
set(CMAKE_CXX_STANDARD 14)

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

set (CMAKE_CXX_FLAGS "-std=gnu++11")

# Creates binary.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/cluon-complete.hpp)