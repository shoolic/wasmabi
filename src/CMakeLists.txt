cmake_minimum_required(VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Og -g -Wshadow -Wall -pedantic -Wextra -Wold-style-cast")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fdevirtualize -finline-functions -funroll-loops")
set(EXECUTABLE_NAME ${PROJECT_NAME})


# MACRO(SUBDIRLIST result curdir)
#   FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
#   SET(dirlist "")
#   FOREACH(child ${children})
#     IF(IS_DIRECTORY ${curdir}/${child})
#       LIST(APPEND dirlist ${child})
#     ENDIF()
#   ENDFOREACH()
#   SET(${result} ${dirlist})
# ENDMACRO()

# SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/src)
# FOREACH(subdir ${SUBDIRS})
#   include_directories(${subdir})
# ENDFOREACH()

file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "*.hpp")

list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
add_library(${PROJECT_NAME}Lib STATIC ${SOURCE_FILES} ${INCLUDE_FILES})


add_executable(${EXECUTABLE_NAME} "main.cpp" ${INCLUDE_FILES})
target_link_libraries(${EXECUTABLE_NAME} 
${PROJECT_NAME}Lib
${EXTERNAL_LIBS})