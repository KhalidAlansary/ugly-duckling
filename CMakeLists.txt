cmake_minimum_required(VERSION 3.28)
project(ugly-duckling)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(xml_editor ${SOURCES})
