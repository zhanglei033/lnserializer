#pragma once
// win32
#if defined(WIN64) || defined(_WIN64) || defined(WIN32) || defined(_WIN32)
#define LN_PLATFORM_WIN32 1
// linux
#elif defined(linux) || defined(__linux) || defined(__linux__)
#define LN_PLATFORM_LINUX 1
// android
#elif defined(ANDROID) || defined(__ANDROID__)
#define LN_PLATFORM_ANDROID 1
// apple
#elif defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#include <TargetConditionals.h>
// mac
#if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#define LN_PLATFORM_MAC 1
// ios
#elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define LN_PLATFORM_IOS 1
#endif
// 其他
#else
#if __has_include("sdkconfig.h")
#include "sdkconfig.h"
#ifdef CONFIG_IDF_TARGET
#define LN_PLATFORM_ESP32 1
#else
#error("unknown platform")
#endif

#else
#error("unknown platform")
#endif
#endif

#if defined(__clang__)
#if defined __apple_build_version__
#define LN_COMPILER_APPLECLANG 1
#else
#define LN_COMPILER_CLANG 1
#endif
#define LN_COMPILER_VERSION (((__clang_major__) * 100) + (__clang_minor__ * 10) + __clang_patchlevel__)
#elif defined(__GNUC__)
#define LN_COMPILER_GNUC    1
#define LN_COMPILER_VERSION (((__GNUC__) * 100) + (__GNUC_MINOR__ * 10) + __GNUC_PATCHLEVEL__)
#elif defined(_MSC_VER)
#define LN_COMPILER_MSVC    1
#define LN_COMPILER_VERSION _MSC_VER
#else
#error "unknown compiler."
#endif

#ifndef LN_APP_NDEBUG
#define LN_APP_NDEBUG 0
#endif

#ifdef __cplusplus
#ifndef EXTERN_C
#define EXTERN_C extern "C"
#endif
#ifndef BEGIN_EXTERN_C
#define BEGIN_EXTERN_C extern "C" {
#endif
#ifndef END_EXTERN_C
#define END_EXTERN_C }
#endif
#else
#ifndef EXTERN_C
#define EXTERN_C
#endif
#ifndef BEGIN_EXTERN_C
#define BEGIN_EXTERN_C
#endif
#ifndef END_EXTERN_C
#define END_EXTERN_C
#endif
#endif

typedef float  float32_t;
typedef double float64_t;

// ANSI C
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if __has_include(<sys/stat.h>)
#include <sys/stat.h>
#define LN_HAS_SYS_STAT 1
#endif

#if __has_include(<sys/time.h>)
#include <sys/time.h>
#define LN_HAS_SYS_TIME 1
#endif

#if __has_include(<fcntl.h>)
#include <fcntl.h>
#define LN_HAS_SYS_FCNTL 1
#endif

#if __has_include(<pthread.h>)
#include <pthread.h>
#define LN_HAS_PTHREAD 1
#endif

#if __has_include(<endian.h>)
#include <endian.h>
#define LN_HAS_ENDIAN 1
#endif

#if __has_include(<sys/endian.h>)
#include <sys/endian.h>
#define LN_HAS_SYS_ENDIAN 1
#endif

#if __has_include(<unistd.h>)
#include <unistd.h>
#define LN_HAS_UNISTD 1
#endif

#if __has_include(<sys/eventfd.h>)
#include <sys/eventfd.h>
#define LN_HAS_SYS_EVNETFD 1
#endif

#if __has_include(<sys/socket.h>)
#include <sys/socket.h>
#define LN_HAS_SYS_SOCKET 1
#endif

#ifdef __cplusplus
#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <exception>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <set>
#include <shared_mutex>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#endif

// windows系统相关头文件
#if LN_PLATFORM_WIN32
#pragma warning(disable : 4996) // 禁止Windows下警告：This function or variable may be unsafe.

#include <winsock2.h> // socket
#include <ws2tcpip.h> // for inet_pton,inet_ntop

#include <windows.h> //

#include <direct.h>  // for mkdir,rmdir,chdir,getcwd
#include <io.h>      // for open,close,read,write,lseek,tell
#include <process.h> // for getpid,exec

#undef min
#undef max

// esp32系统相关头文件
#elif LN_PLATFORM_ESP32
#include <arpa/inet.h>
#include <dirent.h>
#include <esp_log.h>
#include <esp_mac.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/portmacro.h>
#include <freertos/task.h>
#include <lwip/api.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/socket.h>

// 类unix平台
#elif
#include <dirent.h> // for mkdir,rmdir,chdir,getcwd
// socket
#include <arpa/inet.h>
#include <netdb.h> // for gethostbyname
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/select.h>
#include <sys/socket.h>
#endif

// win32平台
#if LN_PLATFORM_WIN32
static inline int lnsleep(uint64_t s)
{
    Sleep((DWORD)s * 1000);
    return 0;
}
static inline int lnmsleep(uint64_t ms)
{
    Sleep((DWORD)ms);
    return 0;
}
static inline int lnusleep(uint64_t us)
{
    Sleep((DWORD)us / 1000);
    return 0;
}
static inline void lnrestart()
{
}
// 类unix平台
#else

static inline int lnsleep(uint64_t s)
{
    return usleep(s * 1000000UL);
}
static inline int lnmsleep(uint64_t ms)
{
    return usleep(ms * 1000UL);
}
static inline int lnusleep(uint64_t us)
{
    return usleep(us);
}
static inline void lnrestart()
{
#if LN_PLATFORM_ESP32
    esp_restart();
#endif
}
#endif

// 统一C++各版本的不同
#ifdef __cplusplus
#if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
#define LN_CPP_LANGUAGE _MSVC_LANG
#else
#define LN_CPP_LANGUAGE __cplusplus
#endif
#else
#define LN_CPP_LANGUAGE 0L
#endif // __cplusplus

#define LN_HAS_CPP11 (LN_CPP_LANGUAGE >= 201103L)
#define LN_HAS_CPP14 (LN_CPP_LANGUAGE > 201103L)
#define LN_HAS_CPP17 (LN_CPP_LANGUAGE > 201402L)
#define LN_HAS_CPP20 (LN_CPP_LANGUAGE > 201703L)
#define LN_HAS_CPP23 (LN_CPP_LANGUAGE > 202002L)

#if LN_HAS_CPP11
#define LN_CONSTEXPR11 constexpr
#else
#define LN_CONSTEXPR11
#endif

#if LN_HAS_CPP14
#define LN_CONSTEXPR14 constexpr
#else
#define LN_CONSTEXPR14
#endif

#if LN_HAS_CPP17
#define LN_CONSTEXPR17 constexpr
#else
#define LN_CONSTEXPR17
#endif

#if LN_HAS_CPP20
#define LN_CONSTEXPR20 constexpr
#else
#define LN_CONSTEXPR20
#endif

#if LN_HAS_CPP20
#define LN_NODISCARD [[nodiscard]]
#elif defined(_HAS_NODISCARD)
#define LN_NODISCARD _NODISCARD
#else
#define LN_NODISCARD
#endif

#if LN_HAS_CPP11
#define LN_NOEXCEPT noexcept
#define LN_NORETURN [[noreturn]]
#else
#define LN_NOEXCEPT
#define LN_NORETURN
#endif

#ifdef __has_cpp_attribute
#define LN_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define LN_HAS_CPP_ATTRIBUTE(x) 0
#endif

#ifdef __has_c_attribute
#define LN_HAS_C_ATTRIBUTE(x) __has_c_attribute(x)
#else
#define LN_HAS_C_ATTRIBUTE(x) 0
#endif

#ifdef __has_attribute
#define LN_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#define LN_HAS_ATTRIBUTE(x) 0
#endif

#if LN_HAS_CPP_ATTRIBUTE(fallthrough)
#define LN_FALLTHROUGH [[fallthrough]]
#elif LN_HAS_C_ATTRIBUTE(fallthrough)
#define LN_FALLTHROUGH [[fallthrough]]
#elif LN_HAS_ATTRIBUTE(fallthrough)
#define LN_FALLTHROUGH __attribute__((fallthrough))
#else
#define LN_FALLTHROUGH
#endif

#define LN_BIT31         0x80000000
#define LN_BIT30         0x40000000
#define LN_BIT29         0x20000000
#define LN_BIT28         0x10000000
#define LN_BIT27         0x08000000
#define LN_BIT26         0x04000000
#define LN_BIT25         0x02000000
#define LN_BIT24         0x01000000
#define LN_BIT23         0x00800000
#define LN_BIT22         0x00400000
#define LN_BIT21         0x00200000
#define LN_BIT20         0x00100000
#define LN_BIT19         0x00080000
#define LN_BIT18         0x00040000
#define LN_BIT17         0x00020000
#define LN_BIT16         0x00010000
#define LN_BIT15         0x00008000
#define LN_BIT14         0x00004000
#define LN_BIT13         0x00002000
#define LN_BIT12         0x00001000
#define LN_BIT11         0x00000800
#define LN_BIT10         0x00000400
#define LN_BIT9          0x00000200
#define LN_BIT8          0x00000100
#define LN_BIT7          0x00000080
#define LN_BIT6          0x00000040
#define LN_BIT5          0x00000020
#define LN_BIT4          0x00000010
#define LN_BIT3          0x00000008
#define LN_BIT2          0x00000004
#define LN_BIT1          0x00000002
#define LN_BIT0          0x00000001
#define LN_BIT_MASK(num) (1 << (num))
