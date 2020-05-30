cmake_minimum_required(VERSION 3.8)

project(wasmabi VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
