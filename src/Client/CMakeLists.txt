# COMPILATION FOR CLIENT

include_directories(include src src/Client)
add_executable(client
        ../Ecs/Events.cpp
        Menu.cpp
        Client.cpp
        Game.cpp
        main.cpp
        Button.cpp
)
target_include_directories(client PRIVATE cxx_std_20)
target_include_directories(client PRIVATE _deps/sfml-src/include)
set(BUILD_SHARED_LIBS FALSE)
target_link_libraries(client
    PRIVATE
        sfml-audio
        sfml-graphics
        sfml-system
        sfml-window
        asio
        ${CMAKE_SOURCE_DIR}/libecs.a
)
target_compile_options(client PRIVATE -Wall)

if(UNIX)
    add_custom_command(TARGET client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:client>
            ${CMAKE_SOURCE_DIR}/
    )
endif()
