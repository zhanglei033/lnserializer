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
add_subdirectory(benchmark/third_party/protobuf)

set(CUR_HEADER benchmark/protobuf/test.pb.h)
set(CUR_SOURCE 
	benchmark/benchmark.cpp 
	benchmark/protobuf/test.pb.cc
)
add_executable(${PROJECT_NAME} ${CUR_SOURCE} ${CUR_HEADER})
include_directories(benchmark/third_party/protobuf/src)
include_directories(benchmark/third_party/protobuf/third_party/abseil-cpp)
target_link_libraries(${PROJECT_NAME}
	libprotoc
	libprotobuf
	absl::absl_check
    absl::absl_log
    absl::algorithm
    absl::base
    absl::bind_front
    absl::bits
    absl::btree
    absl::cleanup
    absl::cord
    absl::core_headers
    absl::debugging
    absl::die_if_null
    absl::dynamic_annotations
    absl::flags
    absl::flat_hash_map
    absl::flat_hash_set
    absl::function_ref
    absl::hash
    absl::if_constexpr
    absl::layout
    absl::log_initialize
    absl::log_globals
    absl::log_severity
    absl::memory
    absl::node_hash_map
    absl::node_hash_set
    absl::optional
    absl::random_distributions
    absl::random_random
    absl::span
    absl::status
    absl::statusor
    absl::strings
    absl::synchronization
    absl::time
    absl::type_traits
    absl::utility
    absl::variant
)



foreach(COMPONENT_PATH ${COMPONENT_PATHS})
	message("add_subdirectory:${COMPONENT_PATH}")
	add_subdirectory(${COMPONENT_PATH})
endforeach()
set(CUR_SOURCE lnserializer_test.cpp)
set(CUR_HEADER lnserializer.h)

add_executable(lnserializer_test ${CUR_SOURCE} ${CUR_HEADER})
include_all()
target_link_libraries(lnserializer_test googletest)

