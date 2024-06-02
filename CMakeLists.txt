cmake_minimum_required(VERSION 3.8)

include(cmake/funcs.cmake)
include(cmake/platform.cmake)
include(cmake/vars.cmake)
project(test VERSION 1.0)
# C++17 C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")

foreach(COMPONENT_PATH ${COMPONENT_PATHS})
	message("add_subdirectory:${COMPONENT_PATH}")
	add_subdirectory(${COMPONENT_PATH})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CUR_SOURCE test.cpp)
set(CUR_HEADER lnserializer.h)

add_executable(${PROJECT_NAME} ${CUR_SOURCE} ${CUR_HEADER})
include_all()
target_link_libraries(${PROJECT_NAME} googletest)

