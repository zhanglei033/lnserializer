if(DEFINED IDF_PATH) # ESP32编译

file(GLOB CUR_SOURCE *.c *.cpp)
idf_component_register(SRCS ${CUR_SOURCE}
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES)
else() # 其他编译

cmake_minimum_required (VERSION 3.8)
project(ln3rd VERSION 1.0)

# C++20 C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 编译程序
foreach(3RD_PATH ${COMPONENT_3RD_PATHS})
	message("add_subdirectory:${3RD_PATH}")
	add_subdirectory(${3RD_PATH})
endforeach()
endif()


