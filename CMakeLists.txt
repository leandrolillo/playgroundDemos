#cmake/compiler options
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#put libraries in lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(FetchContent) #Enable FetchContent for cmake git dependencies

#project
project(playgroundDemos VERSION 1.0.0 LANGUAGES C CXX)

#sources
add_subdirectory(src)

#test
include(CTest) #Enable CTest - unfortunately this has to be on the main CMakeLists file or it will not detect tests.
enable_testing()
add_subdirectory(test)