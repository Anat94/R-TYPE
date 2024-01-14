# COMPILATION FOR ECS

add_library(ecs STATIC
                Events.cpp
        )
target_link_libraries(ecs asio)
target_include_directories(ecs PRIVATE cxx_std_20)
target_include_directories(ecs PRIVATE _deps/sfml-src/include)

if(UNIX)
    add_custom_command(TARGET ecs POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:ecs>
            ${CMAKE_SOURCE_DIR}/
    )
endif()