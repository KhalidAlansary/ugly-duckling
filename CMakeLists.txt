cmake_minimum_required(VERSION 3.30)
cmake_policy(SET CMP0167 NEW)
project(ugly-duckling VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GVC REQUIRED IMPORTED_TARGET libgvc)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(include ${Boost_INCLUDE_DIRS})

set(SOURCES
    src/common.cpp
    src/converter.cpp
    src/draw.cpp
    src/main.cpp
    src/minifier.cpp
    src/most_active.cpp
    src/most_followed.cpp
    src/parser.cpp
    src/prettifier.cpp
    src/search.cpp
    src/suggester.cpp
    src/util.cpp
    src/commands/compressor.cpp
    src/commands/decompressor.cpp
    src/commands/draw.cpp
    src/commands/format.cpp
    src/commands/json.cpp
    src/commands/mini.cpp
    src/commands/most_active.cpp
    src/commands/most_influencer.cpp
    src/commands/mutual.cpp
    src/commands/search.cpp
    src/commands/suggest.cpp
    src/commands/verify.cpp
    src/gui/gui.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.ui
    include/mainwindow.hpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(xml_editor ${SOURCES})
target_link_libraries(
    xml_editor
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        gvc
        cgraph
)

include(GNUInstallDirs)
install(
    TARGETS xml_editor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
