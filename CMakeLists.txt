cmake_minimum_required (VERSION 2.6)
project (carpet)
set(CMAKE_CXX_STANDARD 14)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
SET(CMAKE_ASM_COMPILER arm-linux-gnueabihf-gcc)
SET(CMAKE_SYSTEM_PROCESSOR arm)

find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

## Strict warning level
IF (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
  # Use the highest warning level for visual studio.
  SET(CMAKE_CXX_WARNING_LEVEL 4)
  IF (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  ELSE (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  ENDIF (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")

  SET(CMAKE_C_WARNING_LEVEL 4)
  IF (CMAKE_C_FLAGS MATCHES "/W[0-4]")
    STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  ELSE (CMAKE_C_FLAGS MATCHES "/W[0-4]")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  ENDIF (CMAKE_C_FLAGS MATCHES "/W[0-4]")

ELSEIF (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC)
  # use -Wall for gcc
  IF(NOT CMAKE_CXX_FLAGS MATCHES "-Wall$")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  ENDIF(NOT CMAKE_CXX_FLAGS MATCHES "-Wall$")

  IF(NOT CMAKE_C_FLAGS MATCHES "-Wall$")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  ENDIF(NOT CMAKE_C_FLAGS MATCHES "-Wall$")

ELSE (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
  MESSAGE(STATUS "Unknown build tool, cannot set warning flags for your")
ENDIF (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")

#ADD_DEFINITIONS("-march=armv6")
#add_definitions("-mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marm")

# rdynamic means the backtrace should work
IF (CMAKE_BUILD_TYPE MATCHES "Debug")
   add_definitions(-rdynamic)
ENDIF()

 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/header/carpetConfig.h.in"
  "${PROJECT_BINARY_DIR}/header/carpetConfig.h"
)
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}/src/")


add_executable(carpet src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${CLUON_LIBRARIES})

include(CTest)

