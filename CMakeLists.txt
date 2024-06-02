cmake_minimum_required(VERSION 3.8)

include(cmake/funcs.cmake)
include(cmake/platform.cmake)
include(cmake/vars.cmake)
project(lnserializer_benchmark VERSION 1.0)
# C++17 C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 编译程序
# add_subdirectory(benchmark/3rd/protobuf)

set(CUR_HEADER benchmark/protobuf/test.pb.h)
set(CUR_SOURCE 
	benchmark/benchmark.cpp 
	benchmark/protobuf/test.pb.cc
)
add_executable(${PROJECT_NAME} ${CUR_SOURCE} ${CUR_HEADER})
include_directories(benchmark/3rd/protobuf/src)
include_directories(benchmark/3rd/protobuf/third_party/abseil-cpp)

foreach(COMPONENT_PATH ${COMPONENT_PATHS})
	message("add_subdirectory:${COMPONENT_PATH}")
	add_subdirectory(${COMPONENT_PATH})
endforeach()
set(CUR_SOURCE lnserializer_test.cpp)
set(CUR_HEADER lnserializer.h)

add_executable(lnserializer_test ${CUR_SOURCE} ${CUR_HEADER})
include_all()
target_link_libraries(lnserializer_test googletest)

