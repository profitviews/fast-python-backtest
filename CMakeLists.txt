cmake_minimum_required(VERSION 3.22)

project(fast-python-backtest
    VERSION 0.0.1
    DESCRIPTION "An example project demonstrating how to bind python to C++"
    LANGUAGES CXX
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
find_package(Arrow REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)