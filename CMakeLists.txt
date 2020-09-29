cmake_minimum_required(VERSION 3.15)
project(schematics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing(true)

add_subdirectory(schematics)
add_subdirectory(vendor)
