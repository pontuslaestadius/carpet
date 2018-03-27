cmake_minimum_required(VERSION 3.2)
project(carpet)
set(CMAKE_CXX_STANDARD 11)

# Enables cTest
ENABLE_TESTING()
include(CTest)
INCLUDE(Dart)

# Include Cluon.
find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

# Convert messages.odvd to messages.cpp to handle protobuff.
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/messages.cpp
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMAND cluon-msc --cpp-sources --cpp-add-include-file=messages.hpp --out=${CMAKE_BINARY_DIR}/messages.cpp ${CMAKE_CURRENT_SOURCE_DIR}../../../messages.odvd
                   COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/messages.hpp ${CMAKE_CURRENT_SOURCE_DIR}../../../messages.odvd
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../../../messages.odvd)
include_directories(SYSTEM ${CMAKE_BINARY_DIR})

# Creates binary.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_BINARY_DIR}/messages.cpp)
target_link_libraries(${PROJECT_NAME} ${CLUON_LIBRARIES})
