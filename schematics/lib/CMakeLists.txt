cmake_minimum_required(VERSION 3.15)
project(schema_libs LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(schema libschema/libschema.cxx
    libschema/schema/schema.hxx
    libschema/schema/schema.cxx
    libschema/schema/params.cxx
    libschema/schema/params.hxx
    libschema/schema/units.cxx
    libschema/schema/units.hxx
    libschema/schema/reader.cxx libschema/schema/reader.hxx
    libschema/schema/writer.cxx libschema/schema/writer.hxx
)

target_include_directories(schema PUBLIC libschema)

target_link_libraries(schema PRIVATE Qt5::Core)