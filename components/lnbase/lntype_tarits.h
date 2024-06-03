#pragma once
#include "lnplatform.h"
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