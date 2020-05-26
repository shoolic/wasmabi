cmake_minimum_required(VERSION 3.8)

project(wasmabi VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
find_package(LLVM 10.0.0 REQUIRED CONFIG)

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
