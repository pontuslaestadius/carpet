cmake_minimum_required(VERSION 3.2)
project(obstacle-avoidance)
set(CMAKE_CXX_STANDARD 11)

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

# Creates binary.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} ${CLUON_LIBRARIES})
