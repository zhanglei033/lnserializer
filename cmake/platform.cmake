# 判断平台
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message(">> Platform: Windows")
    # TODO: windows平台特定
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(">> Platform: Linux")
    # TODO: Linux平台特定
else()
    message(">> Platform: Unknown")
    message(">> host CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
endif()

# 判断编译器
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(">> Compiler: Clang")
    # TODO: clang编译器特定
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(">> Compiler: GCC")
    # TODO: gcc编译器特定
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message(">> Compiler: MSVC")
    # TODO: msvc编译器特定
    # 将/md替换为mt
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
    # _CRT_SECURE_NO_WARNINGS
else()
    message(">> Compiler: Unknown")
    message(">> host CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
    if(DEFINED IDF_PATH)
        message(">> Compiler: ESP32 GCC")
    endif()
endif()