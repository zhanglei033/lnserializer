#pragma once
#include "lnconst_string.h"

namespace ln {
inline constexpr auto type_string_end = '\0';
// 作为一个类型存在的字符串
template <char... C>
struct type_string
{
    static constexpr auto to_string()
    {
        return const_string_create({C...});
    }
};
template <class TypeString, char Back>
struct type_string_push_back;
template <template <char...> class TypeString, char... C, char Back>
struct type_string_push_back<TypeString<C...>, Back>
{
    using type = std::conditional_t<Back == type_string_end, TypeString<C...>, TypeString<C..., Back>>;
};
template <class TypeString, char Back>
using type_string_push_back_t = typename type_string_push_back<TypeString, Back>::type;

template <class TypeString, char... C>
struct type_string_create_helper
{
    using type = TypeString;
};
template <class TypeString, char First, char... Rest>
struct type_string_create_helper<TypeString, First, Rest...>
{
    using type = std::conditional_t<
        First == type_string_end,
        TypeString,
        std::conditional_t<
            sizeof...(Rest) == 0,
            type_string_push_back_t<TypeString, First>,
            typename type_string_create_helper<type_string_push_back_t<TypeString, First>, Rest...>::type>>;
};

template <char... C>
struct type_string_create
{
    using type = typename type_string_create_helper<type_string<>, C...>::type;
};

template <class TypeString>
struct type_string_size;

template <template <char...> class TypeString, char... C>
struct type_string_size<TypeString<C...>> : std::integral_constant<size_t, sizeof...(C)>
{
};

template <class TypeString>
struct type_string_data;

template <template <char...> class TypeString, char... C>
struct type_string_data<TypeString<C...>>
{
    static constexpr auto value = const_string_create({C...});
};

template <std::size_t Idx, std::size_t N>
constexpr char type_string_get_char(const char (&str)[N])
{
    return str[Idx < N ? Idx : N - 1];
}

#define LN_TSTR_GET_CHAR(x, y, str)         ln::type_string_get_char<0x##x##y>(str)
#define LN_TSTR_EXPAND_STR_LEVEL1_1(x, str) LN_TSTR_GET_CHAR(x, 0, str), LN_TSTR_GET_CHAR(x, 1, str), LN_TSTR_GET_CHAR(x, 2, str), LN_TSTR_GET_CHAR(x, 3, str)
#define LN_TSTR_EXPAND_STR_LEVEL1_2(x, str) LN_TSTR_GET_CHAR(x, 4, str), LN_TSTR_GET_CHAR(x, 5, str), LN_TSTR_GET_CHAR(x, 6, str), LN_TSTR_GET_CHAR(x, 7, str)
#define LN_TSTR_EXPAND_STR_LEVEL1_3(x, str) LN_TSTR_GET_CHAR(x, 8, str), LN_TSTR_GET_CHAR(x, 9, str), LN_TSTR_GET_CHAR(x, A, str), LN_TSTR_GET_CHAR(x, B, str)
#define LN_TSTR_EXPAND_STR_LEVEL1_4(x, str) LN_TSTR_GET_CHAR(x, C, str), LN_TSTR_GET_CHAR(x, D, str), LN_TSTR_GET_CHAR(x, E, str), LN_TSTR_GET_CHAR(x, F, str)
#define LN_TSTR_EXPAND_STR_LEVEL1(x, str)   LN_TSTR_EXPAND_STR_LEVEL1_1(x, str), LN_TSTR_EXPAND_STR_LEVEL1_2(x, str), LN_TSTR_EXPAND_STR_LEVEL1_3(x, str), LN_TSTR_EXPAND_STR_LEVEL1_4(x, str)
#define LN_TSTR_EXPAND_STR_16(str)          LN_TSTR_EXPAND_STR_LEVEL1(0, str)

#define LN_TSTR_EXPAND_STR_LEVEL2_1(str)    LN_TSTR_EXPAND_STR_LEVEL1(0, str), LN_TSTR_EXPAND_STR_LEVEL1(1, str), LN_TSTR_EXPAND_STR_LEVEL1(2, str), LN_TSTR_EXPAND_STR_LEVEL1(3, str)
#define LN_TSTR_EXPAND_STR_LEVEL2_2(str)    LN_TSTR_EXPAND_STR_LEVEL1(4, str), LN_TSTR_EXPAND_STR_LEVEL1(5, str), LN_TSTR_EXPAND_STR_LEVEL1(6, str), LN_TSTR_EXPAND_STR_LEVEL1(7, str)
#define LN_TSTR_EXPAND_STR_LEVEL2_3(str)    LN_TSTR_EXPAND_STR_LEVEL1(8, str), LN_TSTR_EXPAND_STR_LEVEL1(9, str), LN_TSTR_EXPAND_STR_LEVEL1(A, str), LN_TSTR_EXPAND_STR_LEVEL1(B, str)
#define LN_TSTR_EXPAND_STR_LEVEL2_4(str)    LN_TSTR_EXPAND_STR_LEVEL1(C, str), LN_TSTR_EXPAND_STR_LEVEL1(D, str), LN_TSTR_EXPAND_STR_LEVEL1(E, str), LN_TSTR_EXPAND_STR_LEVEL1(F, str)
#define LN_TSTR_EXPAND_STR_LEVEL2(str)      LN_TSTR_EXPAND_STR_LEVEL2_1(str), LN_TSTR_EXPAND_STR_LEVEL2_2(str), LN_TSTR_EXPAND_STR_LEVEL2_3(str), LN_TSTR_EXPAND_STR_LEVEL2_4(str)
#define LN_TSTR_EXPAND_STR_256(str)         LN_TSTR_EXPAND_STR_LEVEL2(str)

#define LN_TSTR(str)                        ln::type_string_create<LN_TSTR_EXPAND_STR_256(str)>::type

} // namespace ln