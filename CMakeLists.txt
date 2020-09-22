cmake_minimum_required(VERSION 3.10)

project(MatrixApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

file(GLOB_RECURSE Sources "src/*.cpp")

include_directories(src)

add_executable(${PROJECT_NAME} ${Sources})