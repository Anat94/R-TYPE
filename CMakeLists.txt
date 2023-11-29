cmake_minimum_required(VERSION 3.5)
project(R-type
        DESCRIPTION "r type project"
        LANGUAGES CXX)

# COMPILATION FOR SERVER

add_executable(server
                src/Server/main.cpp
                src/Server/Server.cpp
        )
include_directories(include src)
target_compile_features(server PRIVATE cxx_std_20)

# COMPILATION FOR CLIENT

add_executable(client
                src/Client/main.cpp
        )
include_directories(include src)
target_compile_features(client PRIVATE cxx_std_20)

# COMPILATION FOR ECS

add_library(ecs SHARED
                src/Ecs/main.cpp
        )
target_include_directories(ecs PRIVATE cxx_std_20)
