if(DEFINED IDF_PATH) # ESP32编译

file(GLOB_RECURSE CUR_SOURCE *.c *.cpp)
idf_component_register(SRCS ${CUR_SOURCE}
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES)
else() # 其他编译

cmake_minimum_required (VERSION 3.8)
project(spdlog VERSION 1.0)

# C++17 C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 编译程序
file(GLOB_RECURSE CUR_SOURCE *.c *.cpp)
file(GLOB_RECURSE CUR_HEADER *.h *.hpp)
if(NOT CUR_SOURCE)
    if(NOT CUR_HEADER)
        message("spdlog source is empty")
    else()
        message("spdlog is only header")
    endif()
else()
    add_library(${PROJECT_NAME} STATIC ${CUR_SOURCE} ${CUR_HEADER})
endif()
include_all()
endif()


