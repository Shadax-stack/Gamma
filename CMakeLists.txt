cmake_minimum_required(VERSION 2.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

project(Gamma)

add_subdirectory(vendor/SDL2)
add_subdirectory(Gamma)
add_subdirectory(Demo)

set(VS_STARTUP_PROJECT Demo)
