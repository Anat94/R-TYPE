# # Define the project

cmake_minimum_required(VERSION 3.5)
project(R-type
        DESCRIPTION "r type project"
        LANGUAGES CXX)


# Install asio
if(NOT DEFINED ASIO_CMAKE_ASIO_SOURCE_DIR)
        set(ASIO_CMAKE_ASIO_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

set(ASIO_CMAKE_ASIO_DEP_DIR "${ASIO_CMAKE_ASIO_SOURCE_DIR}/asio-${ASIO_CMAKE_ASIO_TAG}-src")

include(FetchContent)

FetchContent_Declare(asio
        GIT_REPOSITORY  https://github.com/chriskohlhoff/asio.git
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

# Install boost

if (WIN32)
        set(BOOST_INCLUDE_LIBRARIES thread filesystem system asio program_options)
        set(BOOST_ENABLE_CMAKE ON)

        message(STATUS "Downloading and extracting boost library sources. This will take some time...")
        include(FetchContent)
        Set(FETCHCONTENT_QUIET FALSE)
        FetchContent_Declare(
                Boost
                URL https://github.com/boostorg/boost/releases/download/boost-1.81.0/boost-1.81.0.7z
                USES_TERMINAL_DOWNLOAD TRUE
                GIT_PROGRESS TRUE
                DOWNLOAD_NO_EXTRACT FALSE
                DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )
        FetchContent_MakeAvailable(Boost)
endif()

# Install sfml

message("Avant le DL")
# find_library(X11_LIBRARIES NAMES X11)
# find_library(XRANDR_LIBRARIES NAMES Xrandr)
# find_library(XCURSOR_LIBRARIES NAMES Xcursor)
# find_path(X11_INCLUDE_DIR NAMES X11/Xlib.h)
# # find_package(X11 REQUIRED)
# include_directories(${X11_INCLUDE_DIR})
# # set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
# # set(CMAKE_SUPPRESS_REGENERATION true)
# # include_directories(${X11_INCLUDE_DIR})
# # find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
# # include_directories(${SFML_INCLUDE_DIR})
# # if (WIN32)
# # list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/installed/x64-windows/share/sfml/")
# # endif()
# include(FetchContent)
# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED true)
# FetchContent_Declare(
#         SFML
#         GIT_REPOSITORY https://github.com/SFML/SFML.git
#         GIT_TAG 2.5.1
#         )
# FetchContent_MakeAvailable(SFML)


# cmake_minimum_required(VERSION 3.10)

# include(FetchContent)

# project(2DComputerGraphics)

# set(CMAKE_CXX_STANDARD 17)

set(SFML_VERSION "2.5.1")

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


message("Après le DL")

# COMPILATION FOR SERVER

add_executable(server
                src/Server/Server.cpp
                src/Server/main.cpp
        )
include_directories(include src)
# find_package(asio CONFIG REQUIRED)
# target_link_libraries(server PRIVATE asio::asio)
target_link_libraries(server
  PRIVATE 
  Boost::asio
  Boost::filesystem
  Boost::thread
  Boost::program_options
)
target_compile_features(server PRIVATE cxx_std_20)

# COMPILATION FOR ECS

add_library(ecs
                src/Ecs/main.cpp
        )
target_include_directories(ecs PRIVATE cxx_std_20)

# COMPILATION FOR CLIENT

# add_executable(client
#                 src/Client/main.cpp
#                 src/Client/Game.cpp
#         )
include_directories(include src src/Client)
# set(SFML_USE_STATIC_STD_LIBS TRUE)
# find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
# target_link_libraries(client PRIVATE sfml-audio
#                 sfml-graphics
#                 sfml-system
#                 sfml-window
        # )
# target_link_libraries(client PRIVATE ${CMAKE_SOURCE_DIR}/libecs.a)
# target_compile_features(client PRIVATE cxx_std_20)

add_executable(client
        src/Client/Client.cpp
        src/Client/Game.cpp
        src/Client/main.cpp
        )
        
target_include_directories(client PRIVATE cxx_std_20)
target_link_libraries(client
    PRIVATE
        sfml-audio
        sfml-graphics
        sfml-system
        sfml-window
)

target_link_libraries(client
  PRIVATE 
  Boost::asio
  Boost::filesystem
  Boost::thread
  Boost::program_options
)

# target_link_libraries(client ${CMAKE_DL_LIBS})

target_compile_options(client PRIVATE -Wall)
# find_package(asio CONFIG REQUIRED)
# target_link_libraries(client PRIVATE asio::asio)
