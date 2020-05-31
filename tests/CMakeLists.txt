cmake_minimum_required(VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Tests)
set(TEST_NAME ${PROJECT_NAME}Tests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)


file(GLOB_RECURSE SOURCE_FILES "*.cpp")
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/tests)

add_executable(${TEST_NAME} ${SOURCE_FILES})

find_package(LLVM 10.0.0 REQUIRED CONFIG)
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

target_link_libraries(
  ${TEST_NAME}
  ${PROJECT_NAME}Lib
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

add_test(
  NAME ${TEST_NAME} 
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME}
)