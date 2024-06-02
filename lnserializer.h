#pragma once
/* lnbase begin */
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

#ifndef LN_APP_NDEBUG
#define LN_APP_NDEBUG 1
#endif

typedef float  float32_t;
typedef double float64_t;

// cpp
#include <any>
#include <array>
#include <deque>
#include <forward_list>
#include <functional>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

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
/* lnbase end */

/* lntype_tarits begin */
namespace ln {
/*
 * @brief确定类型参数是否为指针
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_pointer_v = false;
template <class T>
static constexpr bool is_pointer_v<T*> = true;
template <class T>
static constexpr bool is_pointer_v<T* const> = true;
template <class T>
static constexpr bool is_pointer_v<T* volatile> = true;
template <class T>
static constexpr bool is_pointer_v<T* const volatile> = true;

/*
 * @brief 确定类型参数是否为引用
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_reference_v = false;
template <class T>
static constexpr bool is_reference_v<T&> = true;
template <class T>
static constexpr bool is_reference_v<const T&> = true;
template <class T>
static constexpr bool is_reference_v<T&&> = true;

/*
 * @brief 确定类型参数是否为常量限定
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_const_v = false;
template <class T>
static constexpr bool is_const_v<const T> = true;
template <class T>
static constexpr bool is_const_v<const T*> = true;
template <class T>
static constexpr bool is_const_v<T* const> = true;
template <class T>
static constexpr bool is_const_v<const T* const> = true;
template <class T>
static constexpr bool is_const_v<const T&> = true;

/*
 * @brief 判断类型是否是枚举
 *
 * @tparam T
 */
template <class T>
inline constexpr bool is_enum_v = is_enum<T>::value;

/*
 * @brief 判断类型是否是vector的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_vector_v = false;
template <class T>
static constexpr bool is_vector_v<std::vector<T>> = true;
template <class T>
static constexpr bool is_vector_v<const std::vector<T>> = true;
template <class T>
static constexpr bool is_vector_v<std::vector<T>&> = true;
template <class T>
static constexpr bool is_vector_v<const std::vector<T>&> = true;

/*
 * @brief 判断类型是否是basic_string的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_stdstring_v = false;
template <class T>
static constexpr bool is_stdstring_v<std::basic_string<T>> = true;
template <class T>
static constexpr bool is_stdstring_v<const std::basic_string<T>> = true;
template <class T>
static constexpr bool is_stdstring_v<std::basic_string<T>&> = true;
template <class T>
static constexpr bool is_stdstring_v<const std::basic_string<T>&> = true;

/*
 * @brief 判断类型是否是array的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_array_v = false;
template <class T, size_t N>
static constexpr bool is_array_v<std::array<T, N>> = true;
template <class T, size_t N>
static constexpr bool is_array_v<std::array<T, N>&> = true;
template <class T, size_t N>
static constexpr bool is_array_v<const std::array<T, N>> = true;
template <class T, size_t N>
static constexpr bool is_array_v<const std::array<T, N>&> = true;

/*
 * @brief 判断类型是否是list的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_list_v = false;
template <class T>
static constexpr bool is_list_v<std::list<T>> = true;
template <class T>
static constexpr bool is_list_v<std::list<T>&> = true;
template <class T>
static constexpr bool is_list_v<const std::list<T>> = true;
template <class T>
static constexpr bool is_list_v<const std::list<T>&> = true;

/*
 * @brief 判断类型是否是set的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_set_v = false;
template <class T>
static constexpr bool is_set_v<std::set<T>> = true;
template <class T>
static constexpr bool is_set_v<std::set<T>&> = true;
template <class T>
static constexpr bool is_set_v<const std::set<T>> = true;
template <class T>
static constexpr bool is_set_v<const std::set<T>&> = true;

/*
 * @brief 判断类型是否是map的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_map_v = false;
template <class T1, class T2>
static constexpr bool is_map_v<std::map<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_map_v<std::map<T1, T2>&> = true;
template <class T1, class T2>
static constexpr bool is_map_v<const std::map<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_map_v<const std::map<T1, T2>&> = true;

/*
 * @brief 判断类型是否是pair的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_pair_v = false;
template <class T1, class T2>
static constexpr bool is_pair_v<std::pair<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_pair_v<std::pair<T1, T2>&> = true;
template <class T1, class T2>
static constexpr bool is_pair_v<const std::pair<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_pair_v<const std::pair<T1, T2>&> = true;

/*
 * @brief 判断类型是否是tuple的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_tuple_v = false;
template <class... Args>
static constexpr bool is_tuple_v<std::tuple<Args...>> = true;
template <class... Args>
static constexpr bool is_tuple_v<std::tuple<Args...>&> = true;
template <class... Args>
static constexpr bool is_tuple_v<const std::tuple<Args...>> = true;
template <class... Args>
static constexpr bool is_tuple_v<const std::tuple<Args...>&> = true;

/*
 * @brief 判断类型是否是deque的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_deque_v = false;
template <class T>
static constexpr bool is_deque_v<std::deque<T>> = true;
template <class T>
static constexpr bool is_deque_v<std::deque<T>&> = true;
template <class T>
static constexpr bool is_deque_v<const std::deque<T>> = true;
template <class T>
static constexpr bool is_deque_v<const std::deque<T>&> = true;

/*
 * @brief 判断类型是否是forward_list的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_forward_list_v = false;
template <class T>
static constexpr bool is_forward_list_v<std::forward_list<T>> = true;
template <class T>
static constexpr bool is_forward_list_v<std::forward_list<T>&> = true;
template <class T>
static constexpr bool is_forward_list_v<const std::forward_list<T>> = true;
template <class T>
static constexpr bool is_forward_list_v<const std::forward_list<T>&> = true;

/*
 * @brief 判断类型是否是queue的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_queue_v = false;
template <class T>
static constexpr bool is_queue_v<std::queue<T>> = true;
template <class T>
static constexpr bool is_queue_v<std::queue<T>&> = true;
template <class T>
static constexpr bool is_queue_v<const std::queue<T>> = true;
template <class T>
static constexpr bool is_queue_v<const std::queue<T>&> = true;

/*
 * @brief 判断类型是否是priority_queue的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_priority_queue_v = false;
template <class T>
static constexpr bool is_priority_queue_v<std::priority_queue<T>> = true;
template <class T>
static constexpr bool is_priority_queue_v<std::priority_queue<T>&> = true;
template <class T>
static constexpr bool is_priority_queue_v<const std::priority_queue<T>> = true;
template <class T>
static constexpr bool is_priority_queue_v<const std::priority_queue<T>&> = true;

/*
 * @brief 判断类型是否是stack的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_stack_v = false;
template <class T>
static constexpr bool is_stack_v<std::stack<T>> = true;
template <class T>
static constexpr bool is_stack_v<std::stack<T>&> = true;
template <class T>
static constexpr bool is_stack_v<const std::stack<T>> = true;
template <class T>
static constexpr bool is_stack_v<const std::stack<T>&> = true;

/*
 * @brief 判断类型是否是multimap的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_multimap_v = false;
template <class T1, class T2>
static constexpr bool is_multimap_v<std::multimap<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_multimap_v<std::multimap<T1, T2>&> = true;
template <class T1, class T2>
static constexpr bool is_multimap_v<const std::multimap<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_multimap_v<const std::multimap<T1, T2>&> = true;

/*
 * @brief 判断类型是否是unordered_map的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_unordered_map_v = false;
template <class T1, class T2>
static constexpr bool is_unordered_map_v<std::unordered_map<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_unordered_map_v<std::unordered_map<T1, T2>&> = true;
template <class T1, class T2>
static constexpr bool is_unordered_map_v<const std::unordered_map<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_unordered_map_v<const std::unordered_map<T1, T2>&> = true;

/*
 * @brief 判断类型是否是unordered_multimap的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_unordered_multimap_v = false;
template <class T1, class T2>
static constexpr bool is_unordered_multimap_v<std::unordered_multimap<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_unordered_multimap_v<std::unordered_multimap<T1, T2>&> = true;
template <class T1, class T2>
static constexpr bool is_unordered_multimap_v<const std::unordered_multimap<T1, T2>> = true;
template <class T1, class T2>
static constexpr bool is_unordered_multimap_v<const std::unordered_multimap<T1, T2>&> = true;

/*
 * @brief 判断类型是否是multiset的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_multiset_v = false;
template <class T>
static constexpr bool is_multiset_v<std::multiset<T>> = true;
template <class T>
static constexpr bool is_multiset_v<std::multiset<T>&> = true;
template <class T>
static constexpr bool is_multiset_v<const std::multiset<T>> = true;
template <class T>
static constexpr bool is_multiset_v<const std::multiset<T>&> = true;

/*
 * @brief 判断类型是否是unordered_set的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_unordered_set_v = false;
template <class T>
static constexpr bool is_unordered_set_v<std::unordered_set<T>> = true;
template <class T>
static constexpr bool is_unordered_set_v<std::unordered_set<T>&> = true;
template <class T>
static constexpr bool is_unordered_set_v<const std::unordered_set<T>> = true;
template <class T>
static constexpr bool is_unordered_set_v<const std::unordered_set<T>&> = true;

/*
 * @brief 判断类型是否是unordered_multiset的特化
 *
 * @tparam T
 */
template <class T>
static constexpr bool is_unordered_multiset_v = false;
template <class T>
static constexpr bool is_unordered_multiset_v<std::unordered_multiset<T>> = true;
template <class T>
static constexpr bool is_unordered_multiset_v<std::unordered_multiset<T>&> = true;
template <class T>
static constexpr bool is_unordered_multiset_v<const std::unordered_multiset<T>> = true;
template <class T>
static constexpr bool is_unordered_multiset_v<const std::unordered_multiset<T>&> = true;

template <class Func>
struct func_traits
{
    using result_type = void;
    using args_type   = std::tuple<>;
    using class_type  = void;
    using func_type   = std::function<void()>;
};

template <class R, class... Args>
struct func_traits<R (*)(Args...)>
{
    using result_type = R;
    using args_type   = std::tuple<Args...>;
    using class_type  = void;
    using func_type   = std::function<R (*)(Args...)>;
};

template <class R, class C, class... Args>
struct func_traits<R (C::*)(Args...)>
{
    using result_type = R;
    using args_type   = std::tuple<Args...>;
    using class_type  = C;
    using func_type   = std::function<R (C::*)(Args...)>;
};

template <class R, class C, class... Args>
struct func_traits<R (C::*)(Args...) const>
{
    using result_type = R;
    using args_type   = std::tuple<Args...>;
    using class_type  = C;
    using func_type   = std::function<R (C::*)(Args...)>;
};
} // namespace ln
/* lntype_tarits end */

/* lnserializer begin */
#if LN_APP_NDEBUG
#define serialization_check_offset(cont)
#else
#define serialization_check_offset(cont)        \
    if (cont.offset == static_cast<size_t>(-1)) \
        return cont;
#endif

#define LN_SERIALIZER_VERSION "1000"

namespace ln {
namespace serializer_internal {
template <class T, class = std::void_t<>>
struct serialization_container_has_append : std::false_type
{
};

template <class T>
struct serialization_container_has_append<T, std::void_t<decltype(std::declval<T>().append(nullptr, 0))>> : std::true_type
{
};

template <class T, class = std::void_t<>>
struct serialization_container_has_data : std::false_type
{
};

template <class T>
struct serialization_container_has_data<T, std::void_t<decltype(std::declval<T>().data())>> : std::true_type
{
};

template <class T, class = std::void_t<>>
struct serialization_container_has_size : std::false_type
{
};

template <class T>
struct serialization_container_has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type
{
};

template <bool Test, class T>
struct serialization_container_func_return_type
{
    using type1 = decltype(std::declval<T>().data());
    using type2 = decltype(std::declval<T>().size());
};

template <class T>
struct serialization_container_func_return_type<false, T>
{
    using type1 = void;
    using type2 = void;
};

template <class T>
using serialization_container_data_return_type = serialization_container_func_return_type<
    serialization_container_has_data<T>::value && serialization_container_has_size<T>::value,
    T>::type1;

template <class T>
using serialization_container_size_return_type = serialization_container_func_return_type<
    serialization_container_has_data<T>::value && serialization_container_has_size<T>::value,
    T>::type2;

template <class T>
static inline constexpr bool serialization_container_check1 = serialization_container_has_append<T>::value;
template <class T>
static inline constexpr bool serialization_container_check2 = std::is_convertible_v<serialization_container_data_return_type<T>, const void*>;
template <class T>
static inline constexpr bool serialization_container_check3 = std::is_convertible_v<serialization_container_size_return_type<T>, size_t>;
template <class T>
static inline constexpr bool serialization_container_vaild = serialization_container_check1<T>&& serialization_container_check2<T>&& serialization_container_check3<T>;

} // namespace serializer_internal

struct serialization_basic_type
{
    template <class T>
    static constexpr bool    is_basic_type_v  = std::is_arithmetic_v<T>;
    static constexpr uint8_t len_control_bit  = 4;
    static constexpr uint8_t len_control_mask = 0x0F;
    template <uint8_t ByteSize, class = std::enable_if_t<(ByteSize > 0 && ByteSize <= 8)>>
    static constexpr uint64_t byte_over_flow = static_cast<uint64_t>(1) << (ByteSize * 8 - len_control_bit);

    template <class T>
    inline static uint8_t need_byte_size(T value)
    {
        if constexpr (sizeof(T) == 8)
        {
            uint64_t svalue = static_cast<uint64_t>(value);

            if (svalue < byte_over_flow<1>)
                return 0;
            else if (svalue < byte_over_flow<2>)
                return 1;
            else if (svalue < byte_over_flow<3>)
                return 2;
            else if (svalue < byte_over_flow<4>)
                return 3;
            else if (svalue < byte_over_flow<5>)
                return 4;
            else if (svalue < byte_over_flow<6>)
                return 5;
            else if (svalue < byte_over_flow<7>)
                return 6;
            else if (svalue < byte_over_flow<8>)
                return 7;
            else
                return 8;
        }
        else
        {
            uint32_t svalue = static_cast<uint32_t>(value);

            if (svalue < byte_over_flow<1>)
                return 0;
            else if (svalue < byte_over_flow<2>)
                return 1;
            else if (svalue < byte_over_flow<3>)
                return 2;
            else if (svalue < byte_over_flow<4>)
                return 3;
            else
                return 4;
        }
    }

    template <class Container, class T, class = std::enable_if_t<is_basic_type_v<T>>>
    inline static Container& serialize(Container& cont, const T& value)
    {
        static constexpr uint8_t cur_type_size = sizeof(T);
        if constexpr (std::is_same<T, bool>::value
                      || cur_type_size == 1 || cur_type_size == 2
                      || std::is_same<T, float32_t>::value
                      || std::is_same<T, float64_t>::value)
        {
            cont.append(reinterpret_cast<const uint8_t*>(&value), cur_type_size);
            return cont;
        }
        else
        {
            uint8_t byte_size = need_byte_size(value);
            if (byte_size == cur_type_size)
            {
                size_t temp = (static_cast<size_t>(value) << len_control_bit) | byte_size;
                cont.append(reinterpret_cast<const uint8_t*>(&temp), byte_size);
                uint8_t byte = static_cast<uint8_t>(static_cast<size_t>(value) >> (8 * cur_type_size - len_control_bit) & 0xF);
                cont.append(&byte, 1);
                return cont;
            }

            size_t temp = (static_cast<size_t>(value) << len_control_bit) | byte_size;
            cont.append(reinterpret_cast<uint8_t*>(&temp), byte_size + 1);
            return cont;
        }
    }

    template <class Container, class T, class = std::enable_if_t<is_basic_type_v<T>>>
    inline static Container& deserialize(Container& cont, T& value)
    {
        serialization_check_offset(cont);
        static constexpr uint8_t cur_type_size = sizeof(T);
        if constexpr (std::is_same<T, bool>::value
                      || cur_type_size == 1 || cur_type_size == 2
                      || std::is_same<T, float32_t>::value
                      || std::is_same<T, float64_t>::value)
        {
            if (cont.size() < cont.offset + cur_type_size)
            {
                cont.offset = static_cast<size_t>(-1);
                return cont;
            }

            ::memcpy(&value, cont.cur(), cur_type_size);
            cont.offset += cur_type_size;
            return cont;
        }
        else
        {
            uint8_t byte_size = *cont.cur() & len_control_mask;
            if (byte_size > cur_type_size || cont.size() < cont.offset + byte_size)
            {
                cont.offset = static_cast<size_t>(-1);
                return cont;
            }

            if (byte_size == cur_type_size)
            {
                size_t temp = 0;
                ::memcpy(&temp, cont.cur(), cur_type_size);
                temp >>= 4;
                cont.offset += cur_type_size;
                temp |= static_cast<size_t>(*cont.cur()) << (8 * (cur_type_size - 1) + len_control_bit);
                value = temp;
                cont.offset += 1;
                return cont;
            }

            size_t temp = 0;
            ::memcpy(&temp, cont.cur(), byte_size + 1);
            temp >>= len_control_bit;
            temp &= static_cast<size_t>(-1) >> (8 * (7 - byte_size) + len_control_bit);
            value = temp;
            cont.offset += byte_size + 1;
            return cont;
        }
    }
};

struct serialization_stl_type
{
    template <class T>
    static constexpr bool is_stl_type_v = (is_stdstring_v<T> || is_vector_v<T> || is_array_v<T> || is_deque_v<T> || is_list_v<T> || is_queue_v<T> || is_priority_queue_v<T> || is_stack_v<T> || is_map_v<T> || is_multimap_v<T> || is_set_v<T> || is_multiset_v<T> || is_unordered_map_v<T> || is_unordered_multimap_v<T> || is_unordered_set_v<T> || is_unordered_multiset_v<T> || is_pair_v<T> || is_tuple_v<T>);

    // std::string
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::basic_string<T>& value)
    {
        return (cont << value.size()).append(reinterpret_cast<const uint8_t*>(value.data()), value.size());
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::basic_string<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (cont.size() < cont.offset + size || size == 0)
        {
            cont.offset = static_cast<size_t>(-1);
            return cont;
        }

        value.assign(reinterpret_cast<const char*>(cont.cur()), size);
        cont.offset += size;
        return cont;
    }

    // std::array
    template <class Container, class T, size_t N>
    inline static Container& serialize(Container& cont, const std::array<T, N>& value)
    {
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T, size_t N>
    inline static Container& deserialize(Container& cont, std::array<T, N>& value)
    {
        serialization_check_offset(cont);
        if (N == 0)
            return cont;

        for (auto& v : value)
        {
            cont >> v;
            serialization_check_offset(cont);
        }
        return cont;
    }

    // std::vector
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::vector<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::vector<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        value.resize(size);
        for (auto& v : value)
        {
            cont >> v;
            serialization_check_offset(cont);
        }
        return cont;
    }

    // std::deque
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::deque<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::deque<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        value.resize(size);
        for (auto& v : value)
        {
            cont >> v;
            serialization_check_offset(cont);
        }
        return cont;
    }

    // std::list
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::list<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::list<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        value.resize(size);
        for (auto& v : value)
        {
            cont >> v;
            serialization_check_offset(cont);
        }
        return cont;
    }

    // std::queue
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::queue<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::queue<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        value.resize(size);
        for (auto& v : value)
        {
            cont >> v;
            serialization_check_offset(cont);
        }
        return cont;
    }

    // std::priority_queue
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::priority_queue<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::priority_queue<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;
        value.resize(size);
        for (auto& v : value)
        {
            cont >> v;
            serialization_check_offset(cont);
        }
        return cont;
    }

    // std::stack
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::stack<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::stack<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        value.resize(size);
        for (auto& v : value)
        {
            cont >> v;
            serialization_check_offset(cont);
        }
        return cont;
    }

    // std::map
    template <class Container, class K, class T>
    inline static Container& serialize(Container& cont, const std::map<K, T>& value)
    {
        cont << value.size();
        for (auto& [k, v] : value)
            cont << k << v;
        return cont;
    }
    template <class Container, class K, class T>
    inline static Container& deserialize(Container& cont, std::map<K, T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        for (size_t i = 0; i < size; i++)
        {
            K k{};
            T t{};
            cont >> k >> t;
            serialization_check_offset(cont);
            value.insert(std::make_pair(std::move(k), std::move(t)));
        }
        return cont;
    }

    // std::multimap
    template <class Container, class K, class T>
    inline static Container& serialize(Container& cont, const std::multimap<K, T>& value)
    {
        cont << value.size();
        for (auto& [k, v] : value)
            cont << k << v;
        return cont;
    }
    template <class Container, class K, class T>
    inline static Container& deserialize(Container& cont, std::multimap<K, T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        for (size_t i = 0; i < size; i++)
        {
            K k{};
            T v{};
            cont >> k >> v;
            serialization_check_offset(cont);
            value.insert(std::make_pair(std::move(k), std::move(v)));
        }
        return cont;
    }

    // std::set
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::set<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::set<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        for (size_t i = 0; i < size; i++)
        {
            T v{};
            cont >> v;
            serialization_check_offset(cont);
            value.insert(std::move(v));
        }
        return cont;
    }
    // std::multiset
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::multiset<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::multiset<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        for (size_t i = 0; i < size; i++)
        {
            T v{};
            cont >> v;
            serialization_check_offset(cont);
            value.insert(std::move(v));
        }
        return cont;
    }

    // std::unordered_map
    template <class Container, class K, class T>
    inline static Container& serialize(Container& cont, const std::unordered_map<K, T>& value)
    {
        cont << value.size();
        for (auto& [k, v] : value)
            cont << k << v;
        return cont;
    }
    template <class Container, class K, class T>
    inline static Container& deserialize(Container& cont, std::unordered_map<K, T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        for (size_t i = 0; i < size; i++)
        {
            K k{};
            T t{};
            cont >> k >> t;
            serialization_check_offset(cont);
            value.insert(std::make_pair(std::move(k), std::move(t)));
        }
        return cont;
    }
    // std::unordered_multimap
    template <class Container, class K, class T>
    inline static Container& serialize(Container& cont, const std::unordered_multimap<K, T>& value)
    {
        cont << value.size();
        for (auto& [k, v] : value)
            cont << k << v;
        return cont;
    }
    template <class Container, class K, class T>
    inline static Container& deserialize(Container& cont, std::unordered_multimap<K, T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        for (size_t i = 0; i < size; i++)
        {
            K k{};
            T v{};
            cont >> k >> v;
            serialization_check_offset(cont);
            value.insert(std::make_pair(std::move(k), std::move(v)));
        }
        return cont;
    }

    // std::unordered_set
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::unordered_set<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::unordered_set<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        for (size_t i = 0; i < size; i++)
        {
            T v{};
            cont >> v;
            serialization_check_offset(cont);
            value.insert(std::move(v));
        }
        return cont;
    }
    // std::unordered_multiset
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::unordered_multiset<T>& value)
    {
        cont << value.size();
        for (auto& v : value)
            cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::unordered_multiset<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0)
            return cont;

        for (size_t i = 0; i < size; i++)
        {
            T v{};
            cont >> v;
            serialization_check_offset(cont);
            value.insert(std::move(v));
        }
        return cont;
    }
    // std::pair
    template <class Container, class T1, class T2>
    inline static Container& serialize(Container& cont, const std::pair<T1, T2>& value)
    {
        auto& [k, v] = value;
        return cont << k << v;
    }
    template <class Container, class T1, class T2>
    inline static Container& deserialize(Container& cont, std::pair<T1, T2>& value)
    {
        serialization_check_offset(cont);
        auto& [k, v] = value;
        return cont >> k >> v;
    }
    // std::tuple
    template <class Container, class First, class... Rest>
    inline static Container& serialize(Container& cont, const std::tuple<First, Rest...>& value)
    {
        return serialize<Container, std::tuple<First, Rest...>, 0>(cont, value);
    }
    template <class Container, class Tuple, size_t Idx>
    inline static Container& serialize(Container& cont, const Tuple& value)
    {
        if constexpr (Idx == std::tuple_size_v<Tuple>)
        {
            return cont;
        }
        else
        {
            cont << std::get<Idx>(value);
            return serialize<Container, Tuple, Idx + 1>(cont, value);
        }
    }

    template <class Container, class First, class... Rest>
    inline static Container& deserialize(Container& cont, std::tuple<First, Rest...>& value)
    {
        return deserialize<Container, std::tuple<First, Rest...>, 0>(cont, value);
    }

    template <class Container, class Tuple, size_t Idx>
    inline static Container& deserialize(Container& cont, Tuple& value)
    {
        serialization_check_offset(cont);
        if constexpr (Idx == std::tuple_size_v<Tuple>)
        {
            return cont;
        }
        else
        {
            cont >> std::get<Idx>(value);
            return deserialize<Container, Tuple, Idx + 1>(cont, value);
        }
    }
};

struct serialization_serializable_object
{
    template <class T, class = std::void_t<>>
    struct serializable_object_has_serialize_tuple : std::false_type
    {
    };

    template <class T>
    struct serializable_object_has_serialize_tuple<T, std::void_t<decltype(std::declval<T>().serialize_tuple())>> : std::true_type
    {
    };

    template <class T, class = std::void_t<>>
    struct serializable_object_has_deserialize_tuple : std::false_type
    {
    };

    template <class T>
    struct serializable_object_has_deserialize_tuple<T, std::void_t<decltype(std::declval<T>().deserialize_tuple())>> : std::true_type
    {
    };

    template <class T>
    static constexpr bool serializable_object_has_serialize_tuple_func = serializable_object_has_serialize_tuple<T>::value;
    template <class T>
    static constexpr bool serializable_object_has_deserialize_tuple_func = serializable_object_has_deserialize_tuple<T>::value;

    template <bool Test, class T>
    struct serializable_object_tuple_return_type
    {
        using type1 = decltype(std::declval<T>().serialize_tuple());
        using type2 = decltype(std::declval<T>().deserialize_tuple());
    };

    template <class T>
    struct serializable_object_tuple_return_type<false, T>
    {
        using type1 = void;
        using type2 = void;
    };

    template <class T>
    using serializable_object_serialize_tuple_return_type_t = serializable_object_tuple_return_type<
        serializable_object_has_serialize_tuple_func<T> && serializable_object_has_deserialize_tuple_func<T>,
        T>::type1;

    template <class T>
    using serializable_object_deserialize_tuple_return_type_t = serializable_object_tuple_return_type<
        serializable_object_has_serialize_tuple_func<T> && serializable_object_has_deserialize_tuple_func<T>,
        T>::type2;

    template <size_t I, class Tuple>
    inline static constexpr bool serializable_object_check_serialize_tuple_return_type()
    {
        if constexpr (!is_tuple_v<Tuple>)
            return false;
        else if constexpr (std::tuple_size_v<Tuple> == 0)
            return false;
        else if constexpr (std::tuple_size_v<Tuple> == I)
            return true;
        else if constexpr (std::is_lvalue_reference_v<std::tuple_element_t<I, Tuple>> && !std::is_const_v<std::tuple_element_t<I, Tuple>>)
            return serializable_object_check_serialize_tuple_return_type<I + 1, Tuple>();
        else
            return false;
    }

    template <size_t I, class Tuple>
    inline static constexpr bool serializable_object_check_deserialize_tuple_return_type()
    {
        if constexpr (!is_tuple_v<Tuple>)
            return false;
        else if constexpr (std::tuple_size_v<Tuple> == 0)
            return false;
        else if constexpr (std::tuple_size_v<Tuple> == I)
            return true;
        else if constexpr (std::is_lvalue_reference_v<std::tuple_element_t<I, Tuple>>)
            return serializable_object_check_deserialize_tuple_return_type<I + 1, Tuple>();
        else
            return false;
    }

    template <class T>
    struct is_serializable_object : std::conditional_t<
                                        serializable_object_check_serialize_tuple_return_type<
                                            0,
                                            serializable_object_serialize_tuple_return_type_t<T>>()
                                            && serializable_object_check_deserialize_tuple_return_type<
                                                0,
                                                serializable_object_deserialize_tuple_return_type_t<T>>(),
                                        std::true_type,
                                        std::false_type>
    {
    };

    template <class Container, class T>
    struct serializable_object_proxy
    {
        static void serialize(Container& cont, const T& value)
        {
            cont << value.serialize_tuple();
        }

        static void deserialize(Container& cont, T& value)
        {
            auto tuple = value.deserialize_tuple();
            cont >> tuple;
        }
    };
};

struct serialization_core
{
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const T& value)
    {
        if constexpr (serialization_basic_type::is_basic_type_v<T>)
        {
            return serialization_basic_type::serialize(cont, value);
        }
        else if constexpr (serialization_stl_type::is_stl_type_v<T>)
        {
            return serialization_stl_type::serialize(cont, value);
        }
        else if constexpr (serialization_serializable_object::is_serializable_object<T>::value)
        {
            serialization_serializable_object::serializable_object_proxy<Container, T>::serialize(cont, value);
        }
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, T& value)
    {
        if constexpr (serialization_basic_type::is_basic_type_v<T>)
        {
            return serialization_basic_type::deserialize(cont, value);
        }
        else if constexpr (serialization_stl_type::is_stl_type_v<T>)
        {
            return serialization_stl_type::deserialize(cont, value);
        }
        else if constexpr (serialization_serializable_object::is_serializable_object<T>::value)
        {
            serialization_serializable_object::serializable_object_proxy<Container, T>::deserialize(cont, value);
        }

        return cont;
    }

    template <class Container, class T, size_t N>
    inline static Container& serialize(Container& cont, const T (&value)[N])
    {
        for (auto& v : value) cont << v;
        return cont;
    }
    template <class Container, class T, size_t N>
    inline static Container& deserialize(Container& cont, T (&value)[N])
    {
        serialization_check_offset(cont);
        for (auto& v : value)
        {
            serialization_check_offset(cont);
            cont >> v;
        }
        return cont;
    }
};

template <class Container, class = std::enable_if_t<serializer_internal::serialization_container_vaild<Container>>>
struct serialization_container
{
    Container cont;
    size_t    offset = 0;

    serialization_container& append(const uint8_t* elem, size_t count)
    {
        using value_type = Container::value_type;
        cont.append(reinterpret_cast<const value_type*>(elem), count);
        return *this;
    }

    const uint8_t* cur() const
    {
        return reinterpret_cast<const uint8_t*>(cont.data()) + offset;
    }

    size_t size() const
    {
        return static_cast<size_t>(cont.size());
    }

    template <class T>
    inline serialization_container& operator<<(const T& value)
    {
        return serialization_core::serialize(*this, value);
    }

    template <class T, size_t N>
    inline serialization_container& operator<<(const T (&value)[N])
    {
        return serialization_core::serialize(*this, value);
    }

    template <class T>
    inline serialization_container& operator>>(T& value)
    {
        return serialization_core::deserialize(*this, value);
    }

    template <class T, size_t N>
    inline serialization_container& operator>>(T (&value)[N])
    {
        return serialization_core::deserialize(*this, value);
    }
};
} // namespace ln
/* lnserializer end */
