cmake_minimum_required(VERSION 3.8)
project(Lss_lrta_star)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        probcal.hpp)

include_directories(.)

add_executable(Lss_lrta_star ${SOURCE_FILES})