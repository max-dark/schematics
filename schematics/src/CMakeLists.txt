cmake_minimum_required(VERSION 3.15)
project(schema_app LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(schematics)

target_sources(schematics
        PRIVATE
            main.cxx
            ui/mainwindow.cxx ui/mainwindow.hxx
            ui/tools/tool.cxx ui/tools/tool.hxx

            ui/widgets/scheme/schemeview.cxx ui/widgets/scheme/schemeview.hxx
            ui/widgets/scheme/schemescene.cxx ui/widgets/scheme/schemescene.hxx
            ui/widgets/scheme/schemeeditor.cxx ui/widgets/scheme/schemeeditor.hxx
        )
target_link_libraries(schematics PRIVATE schema Qt5::Widgets)