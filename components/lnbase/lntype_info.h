#pragma once
#include "lnconst_string.h"
#include "lnpp.h"
#include "lntype_string.h"

namespace ln {

#if LN_COMPILER_MSVC
#if LN_HAS_CPP11
// auto __cdecl ln::nameof<T>(void) noexcept
static constexpr size_t typename_begin_pos = sizeof("auto __cdecl ln::nameof<") - 1;
static constexpr size_t typename_end_pos   = sizeof(">(void) noexcept") - 1;
#else
// auto __cdecl ln::nameof<[T]>(void)
static constexpr size_t typename_begin_pos = sizeof("auto __cdecl ln::nameof<") - 1;
static constexpr size_t typename_end_pos   = sizeof(">(void)") - 1;
#endif
#define LN_FUNC_NAME __FUNCSIG__

#elif COMPILER_TYPE_TAG == COMPILER_GNUC_TAG
// auto ln::nameof() [with T = TYPE]
static constexpr size_t typename_begin_pos = sizeof("auto ln::nameof() [with T = ") - 1;
static constexpr size_t typename_end_pos   = sizeof("]") - 1;
#define LN_FUNC_NAME __PRETTY_FUNCTION__

#elif COMPILER_TYPE_TAG == COMPILER_CLANG_TAG || COMPILER_TYPE_TAG == COMPILER_APPLECLANG_TAG
// auto ln::nameof() [T = TYPE]
static constexpr size_t typename_begin_pos = sizeof("auto ln::nameof() [T = ") - 1;
static constexpr size_t typename_end_pos   = sizeof("]") - 1;
#define LN_FUNC_NAME __PRETTY_FUNCTION__

#else
#error "compiler not supported"
#endif

template <typename T>
static constexpr auto nameof() LN_NOEXCEPT
{
    constexpr auto full_name = const_string_create(LN_FUNC_NAME);
    return full_name.substr<typename_begin_pos, typename_end_pos>();
}

template <class T>
struct lntype_info;

template <class Type, class... Bases>
struct lntype_info_base
{
    using type            = Type;
    using type_info       = lntype_info<type>;
    using base_type_infos = std::tuple<lntype_info<Bases>...>;

    static constexpr auto type_name = nameof<type>();
    static constexpr auto type_id   = __COUNTER__;
};

template <class T>
struct lntype_info : lntype_info_base<T>
{
};
} // namespace ln

template <class T>
static constexpr auto lntype_info_get_field_infos()
{
    return std::tuple();
}

#define LN_TYPE_INFO_FIELDS_DECL1(T) \
    return std::tuple();

#define LN_TYPE_INFO_FIELDS_DECL2(T, F1) \
    return std::make_tuple(std::make_pair(LN_TSTR(LN_PP_STRING(F1))(), &T::F1));

#define LN_TYPE_INFO_FIELDS_DECL3(T, F1, F2)                                    \
    return std::make_tuple(std::make_pair(LN_TSTR(LN_PP_STRING(F1))(), &T::F1), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F2))(), &T::F2));
#define LN_TYPE_INFO_FIELDS_DECL4(T, F1, F2, F3)                                \
    return std::make_tuple(std::make_pair(LN_TSTR(LN_PP_STRING(F1))(), &T::F1), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F2))(), &T::F2), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F3))(), &T::F3));
#define LN_TYPE_INFO_FIELDS_DECL5(T, F1, F2, F3, F4)                            \
    return std::make_tuple(std::make_pair(LN_TSTR(LN_PP_STRING(F1))(), &T::F1), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F2))(), &T::F2), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F3))(), &T::F3), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F4))(), &T::F4));
#define LN_TYPE_INFO_FIELDS_DECL6(T, F1, F2, F3, F4, F5)                        \
    return std::make_tuple(std::make_pair(LN_TSTR(LN_PP_STRING(F1))(), &T::F1), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F2))(), &T::F2), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F3))(), &T::F3), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F4))(), &T::F4), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F5))(), &T::F5));
#define LN_TYPE_INFO_FIELDS_DECL7(T, F1, F2, F3, F4, F5, F6)                    \
    return std::make_tuple(std::make_pair(LN_TSTR(LN_PP_STRING(F1))(), &T::F1), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F2))(), &T::F2), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F3))(), &T::F3), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F4))(), &T::F4), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F5))(), &T::F5), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F6))(), &T::F6));
#define LN_TYPE_INFO_FIELDS_DECL8(T, F1, F2, F3, F4, F5, F6, F7)                \
    return std::make_tuple(std::make_pair(LN_TSTR(LN_PP_STRING(F1))(), &T::F1), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F2))(), &T::F2), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F3))(), &T::F3), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F4))(), &T::F4), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F5))(), &T::F5), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F6))(), &T::F6), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F7))(), &T::F7));
#define LN_TYPE_INFO_FIELDS_DECL9(T, F1, F2, F3, F4, F5, F6, F7, F8)            \
    return std::make_tuple(std::make_pair(LN_TSTR(LN_PP_STRING(F1))(), &T::F1), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F2))(), &T::F2), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F3))(), &T::F3), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F4))(), &T::F4), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F5))(), &T::F5), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F6))(), &T::F6), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F7))(), &T::F7), \
                           std::make_pair(LN_TSTR(LN_PP_STRING(F8))(), &T::F8));

#define LN_TYPE_INFO_FIELDS_DECL(...)                                                           \
    template <>                                                                                 \
    static constexpr auto lntype_info_get_field_infos<LN_PP_ARGS0(LN_PP_EXPAND(__VA_ARGS__))>() \
    {                                                                                           \
        LN_PP_EXPAND(LN_PP_OVER_LOAD(LN_TYPE_INFO_FIELDS_DECL, __VA_ARGS__)(__VA_ARGS__))       \
    }
