if(DEFINED IDF_PATH) # ESP32编译
set(CUR_SOURCE "src/gtest-all.cc")
idf_component_register(SRCS ${CUR_SOURCE}
                    INCLUDE_DIRS "." "../xbase"
                    PRIV_REQUIRES)
else() # 其他编译

cmake_minimum_required (VERSION 3.8)
project(googletest VERSION 1.0)

# C++17 C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 编译程序
set(CUR_SOURCE "src/gtest-all.cc")
set(CUR_SOURCE ${CUR_SOURCE} ${TEST_SOURCE})
file(GLOB_RECURSE CUR_HEADER *.h *.hpp)
add_library(${PROJECT_NAME} STATIC ${CUR_SOURCE} ${CUR_HEADER})
include_all()
endif()

