cmake_minimum_required(VERSION 3.15)
project(schema_tests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets Xml Test REQUIRED)

enable_testing(true)

add_executable(test_units test_units.cxx)
add_test(NAME test_units COMMAND test_units)
target_link_libraries(test_units PRIVATE schema Qt5::Test)


add_executable(test_params test_params.cxx)
add_test(NAME test_params COMMAND test_params)
target_link_libraries(test_params PRIVATE schema Qt5::Test)


add_executable(test_schema test_schema.cxx)
add_test(NAME test_schema COMMAND test_schema)
target_link_libraries(test_schema PRIVATE schema Qt5::Test)

add_executable(test_calc)
target_sources(test_calc
    PRIVATE
        test_coords/test_calc_main.cxx
        test_coords/test_schemecalculator.cxx
        test_coords/test_schemecalculator.hxx
)

add_test(NAME test_calc COMMAND test_calc)
target_link_libraries(test_calc PRIVATE schema coords Qt5::Test)

add_executable(test_tags)
target_sources(test_tags PRIVATE test_services/test_tags.cxx)
target_link_libraries(test_tags
    PRIVATE
        services
        Qt5::Test
)
add_test(NAME test_tags COMMAND test_tags)
