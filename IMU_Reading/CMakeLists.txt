cmake_minimum_required(VERSION 3.9)
project(IMU_Reading)

set(CMAKE_CXX_STANDARD 11)

add_executable(IMU_Reading rc_imu_reading.cpp)