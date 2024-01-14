# COMPILATION FOR SERVER

add_executable(server
        Server.cpp
        main.cpp
        database.cpp
)

if (UNIX)
        set(CMAKE_CXX_FLAGS "-pthread -ldl")
endif()
target_compile_options(server PRIVATE -ldl)
if (UNIX)
        add_dependencies(server sqlite3_external)
endif()
# add_dependencies(server sqlite)
if (UNIX)
        ExternalProject_Get_Property(sqlite3_external BINARY_DIR)
endif()
target_include_directories(server PRIVATE ${BINARY_DIR})
target_include_directories(server PRIVATE _deps/sfml-src/include)
if (UNIX)
        target_link_libraries(server
        PRIVATE
                sfml-audio
                sfml-graphics
                sfml-system
                sfml-window
                asio
                ${BINARY_DIR}/.libs/libsqlite3.so
                ${CMAKE_SOURCE_DIR}/libecs.a

        )
elseif(WIN32)
        target_link_libraries(server
        PRIVATE
                sfml-audio
                sfml-graphics
                sfml-system
                sfml-window
                asio
                sqlite3
                ${CMAKE_SOURCE_DIR}/src/Ecs/Debug/ecs.lib
        )
endif()

target_compile_features(server PRIVATE cxx_std_20)

if(UNIX)
    add_custom_command(TARGET server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:server>
            ${CMAKE_SOURCE_DIR}/
    )
endif()
