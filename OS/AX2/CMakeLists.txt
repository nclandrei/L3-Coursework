cmake_minimum_required(VERSION 3.3)
project(AX2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    driver.c
    driver.h
    firmware.cl
    Makefile
    README_opencl_local_install
    testbench.c)

add_executable(AX2 ${SOURCE_FILES})