# COMPILATION FOR CLIENT
include_directories(include src src/Client)
add_executable(client
        Client.cpp
        main.cpp
)
target_include_directories(client PRIVATE cxx_std_20)
target_include_directories(client PRIVATE _deps/sfml-src/include)
set(BUILD_SHARED_LIBS FALSE)
if (UNIX)
    set(LIBEC_PATH ${CMAKE_SOURCE_DIR}/libecs.a)
elseif(WIN32)
    set(LIBEC_PATH ${CMAKE_SOURCE_DIR}/src/Ecs/Debug/ecs.lib)
endif()
target_link_libraries(client
    PRIVATE
        sfml-audio
        sfml-graphics
        sfml-system
        sfml-window
        asio
        ${LIBEC_PATH}
)
target_compile_options(client PRIVATE -Wall)

if(UNIX)
    add_custom_command(TARGET client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:client>
            ${CMAKE_SOURCE_DIR}/
    )
endif()
if (WIN32)
        message("COPYING LIBRARIES")
        message(${CMAKE_SOURCE_DIR}/_deps/sfml-build/lib/Debug/sfml-audio-d-2.dll)
        add_custom_command(TARGET client POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/_deps/sfml-build/lib/Debug/sfml-audio-d-2.dll
                ${CMAKE_SOURCE_DIR}/Debug
        )
        add_custom_command(TARGET client POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/_deps/sfml-build/lib/Debug/sfml-graphics-d-2.dll
                ${CMAKE_SOURCE_DIR}/Debug
        )
        add_custom_command(TARGET client POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/_deps/sfml-build/lib/Debug/sfml-system-d-2.dll
                ${CMAKE_SOURCE_DIR}/Debug
        )
        add_custom_command(TARGET client POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/_deps/sfml-build/lib/Debug/sfml-window-d-2.dll
                ${CMAKE_SOURCE_DIR}/Debug
        )
        add_custom_command(TARGET client POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/Client/assets
                ${CMAKE_SOURCE_DIR}/Debug/assets
        )
endif()
