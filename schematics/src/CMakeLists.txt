cmake_minimum_required(VERSION 3.15)
project(schema_app LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

add_executable(schematics WIN32)

target_sources(schematics
        PRIVATE
            main.cxx
            ui/mainwindow.cxx ui/mainwindow.hxx
            ui/tools/tool.cxx ui/tools/tool.hxx

            services/fasade.cxx services/fasade.hxx
            services/database.cxx services/database.hxx

            ui/widgets/numbereditor.cxx ui/widgets/numbereditor.hxx

            ui/widgets/scheme/schemeview.cxx ui/widgets/scheme/schemeview.hxx
            ui/widgets/scheme/schemescene.cxx ui/widgets/scheme/schemescene.hxx
            ui/widgets/scheme/schemeeditor.cxx ui/widgets/scheme/schemeeditor.hxx

            ui/widgets/coords/coordstab.cxx ui/widgets/coords/coordstab.hxx
        )
target_link_libraries(schematics PRIVATE schema coords
    Qt5::Widgets Qt5::Sql)
