cmake_minimum_required(VERSION 3.28)
project(ugly-duckling)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# Boost settings
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# Find required Boost components
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

include_directories(include ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(xml_editor ${SOURCES})
target_link_libraries(
  xml_editor PRIVATE ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
                     ${Boost_PROGRAM_OPTIONS_LIBRARY} gvc cgraph)