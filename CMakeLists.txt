cmake_minimum_required(VERSION 3.8)

project(wasmabi)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Og -g -Wshadow -Wall -pedantic -Wextra -Wold-style-cast")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fdevirtualize -finline-functions -funroll-loops")

set(EXECUTABLE_NAME ${PROJECT_NAME})

file(GLOB SOURCE_FILES "src/*.cpp")
file(GLOB INCLUDE_FILES "src/*.hpp")

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${EXTERNAL_LIBS})