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