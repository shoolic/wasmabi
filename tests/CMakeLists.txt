cmake_minimum_required(VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Tests)
set(TEST_NAME ${PROJECT_NAME}Tests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "*.cpp")

file(GLOB_RECURSE INCLUDE_FILES ${PROJECT_SOURCE_DIR}/src/*.hpp)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${TEST_NAME} ${SOURCE_FILES})


target_link_libraries(
  ${TEST_NAME}
  ${PROJECT_NAME}Lib
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(
  NAME ${TEST_NAME} 
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME}
)