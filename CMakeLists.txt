cmake_minimum_required(VERSION 3.5)
project(R-type
        DESCRIPTION "r type project"
        LANGUAGES CXX)
add_executable(air_type
                src/main.cpp
                )
include_directories(include src)
target_compile_features(air_type PRIVATE cxx_std_20)