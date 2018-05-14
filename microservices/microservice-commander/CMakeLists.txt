cmake_minimum_required(VERSION 3.5)
project(microservice-commander)
set(CMAKE_CXX_STANDARD 14)

#Libcluon dependency.
find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/messages.cpp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND cluon-msc --cpp-sources --cpp-add-include-file=messages.hpp --out=${CMAKE_BINARY_DIR}/messages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd
        COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/messages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/messages.odvd)

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

#Adds executable service which we can use to start the program.
add_executable(${PROJECT_NAME}.Service ${CMAKE_CURRENT_SOURCE_DIR}/commander.cpp ${CMAKE_BINARY_DIR}/messages.cpp)
target_link_libraries(${PROJECT_NAME}.Service ${CLUON_LIBRARIES})
