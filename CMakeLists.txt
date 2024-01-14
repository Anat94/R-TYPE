# # Define the project

cmake_minimum_required(VERSION 3.5)
project(R-type
        DESCRIPTION "r type project"
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_MESSAGE LAZY)
enable_testing()

# Install asio
if(NOT DEFINED ASIO_CMAKE_ASIO_SOURCE_DIR)
        set(ASIO_CMAKE_ASIO_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

set(ASIO_CMAKE_ASIO_DEP_DIR "${ASIO_CMAKE_ASIO_SOURCE_DIR}/asio-${ASIO_CMAKE_ASIO_TAG}-src")

include(FetchContent)

FetchContent_Declare(asio
        GIT_REPOSITORY  https://github.com/chriskohlhoff/asio.git
        # GIT_TAG         "main"
        GIT_SHALLOW     TRUE
        SOURCE_DIR      ${ASIO_CMAKE_ASIO_DEP_DIR}
)

FetchContent_GetProperties(asio)

if(NOT asio_POPULATED)
        message(STATUS "Fetching asio ${ASIO_CMAKE_ASIO_TAG}")

        if(EXISTS "${ASIO_CMAKE_ASIO_DEP_DIR}/asio")
                set(FETCHCONTENT_SOURCE_DIR_ASIO ${ASIO_CMAKE_ASIO_DEP_DIR})
                message(STATUS "asio ${ASIO_CMAKE_ASIO_TAG} source found at ${ASIO_CMAKE_ASIO_DEP_DIR}; skip fetching.")
        endif()

        FetchContent_Populate(asio)
        message(STATUS "Populating asio ${ASIO_CMAKE_ASIO_TAG} done; src at ${asio_SOURCE_DIR}")
endif()

add_library(asio INTERFACE)

target_include_directories(asio
        INTERFACE ${asio_SOURCE_DIR}/asio/include
)

target_compile_definitions(asio
        INTERFACE
        ASIO_STANDALONE
        ASIO_NO_DEPRECATED
)

if(UNIX)
        target_link_libraries(asio
                INTERFACE pthread
        )
elseif(WIN32)
        macro(get_WIN32_WINNT version)
        if (CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
        string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
        if ("${verMajor}" MATCHES "10")
        set(verMajor "A")
        string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
        endif ("${verMajor}" MATCHES "10")
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
        set(${version} "0x${ver}")
        endif(CMAKE_SYSTEM_VERSION)
        endmacro(get_WIN32_WINNT)

        if(NOT DEFINED _WIN32_WINNT)
        get_WIN32_WINNT(ver)
        set(_WIN32_WINNT ${ver})
        endif()

        message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

        target_compile_definitions(asio
        INTERFACE
        _WIN32_WINNT=${_WIN32_WINNT}
        WIN32_LEAN_AND_MEAN
        )
endif()

# Install sfml

set(SFML_VERSION "2.6.1")

FetchContent_Declare(
    sfml
    GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    GIT_TAG        "${SFML_VERSION}"
)

FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
    FetchContent_Populate(sfml)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()


# INSTALL SQLITE3

include(ExternalProject)
if (UNIX)
        ExternalProject_Add(
                sqlite3_external
                URL https://www.sqlite.org/2023/sqlite-autoconf-3440200.tar.gz
                PREFIX ${CMAKE_BINARY_DIR}/sqlite3
                CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/sqlite3/src/sqlite3_external/configure
                BUILD_COMMAND make
                INSTALL_COMMAND ""
        )
        ExternalProject_Get_Property(sqlite3_external SOURCE_DIR)
        include_directories(${SOURCE_DIR})
elseif(WIN32)
        include_directories(sqlite_windows)
        add_library(sqlite3 STATIC sqlite_windows/sqlite3.c sqlite_windows/sqlite3.h sqlite_windows/sqlite3ext.h)
        # add_executable(sqlite sqlite_windows/sqlite3.c sqlite_windows/shell.c sqlite_windows/sqlite3.h sqlite_windows/sqlite3ext.h)
endif()

# INSTALL GOOGLETESTS

FetchContent_Declare(
        googletest
        GIT_REPOSITORY  https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

# FetchContent_GetProperties(googletest)
FetchContent_MakeAvailable(googletest)


if(NOT googletest_POPULATED)
        message(STATUS "Fetching googletest ${googletest_TAG}")

        if(EXISTS "${googletest_SOURCE_DIR}/googletest")
                set(FETCHCONTENT_SOURCE_DIR_GOOGLETEST ${googletest_SOURCE_DIR})
                message(STATUS "googletest ${googletest_TAG} source found at ${googletest_SOURCE_DIR}; skip fetching.")
        endif()

        FetchContent_Populate(googletest)
        message(STATUS "Populating googletest ${googletest_TAG} done; src at ${googletest_SOURCE_DIR}")
endif()


add_subdirectory(src/Ecs)
add_subdirectory(src/Client)
add_subdirectory(src/Server)
add_subdirectory(tests)