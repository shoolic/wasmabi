cmake_minimum_required(VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Og -g -Wshadow -Wall -pedantic -Wextra -Wold-style-cast")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fdevirtualize -finline-functions -funroll-loops")
set(EXECUTABLE_NAME ${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "*.hpp")

find_package(Threads REQUIRED)

find_package(LLVM 10.0.0 REQUIRED CONFIG)
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

find_package(Boost COMPONENTS filesystem REQUIRED)
include_directories( ${Boost_INCLUDE_DIR})


list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
add_library(${PROJECT_NAME}Lib STATIC ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}Lib stdc++fs LLVM ${Boost_LIBRARIES} Threads::Threads)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories( ${Boost_INCLUDE_DIR})

add_executable(${EXECUTABLE_NAME} "main.cpp" ${INCLUDE_FILES})
target_link_libraries(${EXECUTABLE_NAME} 
${PROJECT_NAME}Lib
${EXTERNAL_LIBS}
${Boost_LIBRARIES} 
)