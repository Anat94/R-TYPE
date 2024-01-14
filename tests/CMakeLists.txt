# Create binary for unit_tests

add_executable(my_tests
        TestSparseArray.cpp
        TestZipperIterator.cpp
)

if (UNIX)
        target_link_libraries(my_tests
        PRIVATE
                ecs
                gtest
                gmock
                gmock_main
        )
elseif(WIN32)
        target_link_libraries(my_tests
        PRIVATE
                ecs
                asio
                sfml-audio
                sfml-graphics
                sfml-system
                sfml-window
                gtest
                gmock
                gmock_main
                sqlite3
        )
endif()
add_test(NAME my_tests COMMAND tests)

if (WIN32)
        add_custom_command(TARGET my_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/bin/Debug/gmock_maind.dll
                ${CMAKE_SOURCE_DIR}/Debug
        )
        add_custom_command(TARGET my_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/bin/Debug/gmockd.dll
                ${CMAKE_SOURCE_DIR}/Debug
        )
        add_custom_command(TARGET my_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/bin/Debug/gtest_maind.dll
                ${CMAKE_SOURCE_DIR}/Debug
        )
        add_custom_command(TARGET my_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/bin/Debug/gtestd.dll
                ${CMAKE_SOURCE_DIR}/Debug
        )
endif()