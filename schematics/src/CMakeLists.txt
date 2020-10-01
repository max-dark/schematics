cmake_minimum_required(VERSION 3.15)
project(schema_app LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets Sql REQUIRED)

add_library(services STATIC)

target_link_libraries(services
    PUBLIC schema coords Qt5::Core Qt5::Sql
    PRIVATE snap7_micro_client
)

target_sources(services
    PRIVATE
        services/application.cxx
        services/storage.cxx

        services/alpha.cxx
        services/facade.cxx
        services/database.cxx
        services/machine.cxx
    PUBLIC
        services/application.hxx
        services/storage.hxx

        services/alpha.hxx
        services/facade.hxx
        services/database.hxx
        services/machine.hxx
)

add_library(ui STATIC)

target_link_libraries(ui
    PUBLIC schema coords Qt5::Widgets
)

target_sources(ui
    PRIVATE
        ui/tools/tool.cxx
        ui/widgets/numbereditor.cxx

        ui/widgets/scheme/schemeview.cxx
        ui/widgets/scheme/schemescene.cxx
        ui/widgets/scheme/schemeeditor.cxx

        ui/widgets/coords/coordstab.cxx
    INTERFACE
        ui/tools/tool.hxx
        ui/widgets/numbereditor.hxx

        ui/widgets/scheme/schemeview.hxx
        ui/widgets/scheme/schemescene.hxx
        ui/widgets/scheme/schemeeditor.hxx

        ui/widgets/coords/coordstab.hxx
)

add_executable(schematics WIN32)

target_sources(schematics
    PRIVATE
        main.cxx
        ui/mainwindow.cxx ui/mainwindow.hxx
)

target_link_libraries(schematics
    PRIVATE
        schema coords services ui
)
