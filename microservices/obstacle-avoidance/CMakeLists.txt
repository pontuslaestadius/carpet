# This file is derived from the source code at:
# https://github.com/chalmers-revere/opendlv-device-ultrasonic-srf08/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.2)
project(obstacle-avoidance)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Add current build directory as include directory as it contains generated files.
include_directories(SYSTEM ${CMAKE_BINARY_DIR})
set(OPENDLV_STANDARD_MESSAGE_SET messages.odvd)
set(CLUON_COMPLETE cluon-complete.hpp)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_BINARY_DIR})

# Extract cluon-msc from cluon-complete.hpp.
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/cluon-msc
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/${CLUON_COMPLETE} ${CMAKE_BINARY_DIR}/cluon-complete.hpp
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/cluon-complete.hpp ${CMAKE_BINARY_DIR}/cluon-complete.cpp
    COMMAND ${CMAKE_CXX_COMPILER} -o ${CMAKE_BINARY_DIR}/cluon-msc ${CMAKE_BINARY_DIR}/cluon-complete.cpp -std=c++14 -pthread -D HAVE_CLUON_MSC
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${CLUON_COMPLETE})

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/messages.cpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_BINARY_DIR}/cluon-msc --cpp-sources --cpp-add-include-file=messages.hpp --out=${CMAKE_BINARY_DIR}/messages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${OPENDLV_STANDARD_MESSAGE_SET}
    COMMAND ${CMAKE_BINARY_DIR}/cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/messages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/${OPENDLV_STANDARD_MESSAGE_SET}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${OPENDLV_STANDARD_MESSAGE_SET} ${CMAKE_BINARY_DIR}/cluon-msc)
# Add current build directory as include directory as it contains generated files.
include_directories(SYSTEM ${CMAKE_BINARY_DIR})


# Creates binary.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/cluon-complete.hpp ${CMAKE_BINARY_DIR}/messages.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})