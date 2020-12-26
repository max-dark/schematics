cmake_minimum_required(VERSION 3.15)
# project(schematics LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

enable_testing(true)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
