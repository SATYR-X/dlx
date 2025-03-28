cmake_minimum_required(VERSION 3.10)

project(AlgorithmDLX VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD_LIBRARIES "-stdlib=libc++")

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")


add_executable(AlgorithmDLX ${SOURCES})
