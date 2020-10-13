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
        services/settingstable.cxx

        services/alpha.cxx
        services/alphasupport.cxx
        services/facade.cxx
        services/database.cxx
        services/machine.cxx
        services/tags.cxx
    PUBLIC
        services/application.hxx
        services/storage.hxx
        services/settingstable.hxx

        services/alpha.hxx
        services/alphasupport.hxx
        services/facade.hxx
        services/database.hxx
        services/machine.hxx
        services/tags.hxx
)

target_include_directories(services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(ui STATIC)

target_link_libraries(ui
    PUBLIC schema coords Qt5::Widgets
)

target_sources(ui
    PRIVATE
        ui/resources/ledpix.qrc

        ui/tools/tool.cxx
        ui/widgets/numbereditor.cxx
        ui/widgets/leds.cxx
        ui/widgets/ledlist.cxx

        ui/widgets/scheme/schemeview.cxx
        ui/widgets/scheme/schemescene.cxx
        ui/widgets/scheme/schemeeditor.cxx

        ui/widgets/coords/coordstab.cxx
    INTERFACE
        ui/tools/tool.hxx
        ui/widgets/numbereditor.hxx
        ui/widgets/leds.hxx
        ui/widgets/ledlist.hxx

        ui/widgets/scheme/schemeview.hxx
        ui/widgets/scheme/schemescene.hxx
        ui/widgets/scheme/schemeeditor.hxx

        ui/widgets/coords/coordstab.hxx
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(schematics WIN32)

target_sources(schematics
    PRIVATE
        main.cxx
        ui/mainwindow.cxx ui/mainwindow.hxx
        ui/dialogs/tabledialog.cxx ui/dialogs/tabledialog.hxx
)

target_link_libraries(schematics
    PRIVATE
        schema coords services ui
)
