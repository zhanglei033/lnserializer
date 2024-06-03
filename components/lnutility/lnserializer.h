#pragma once
#include "lntype_info.h"
#include "lntype_tarits.h"

#if LN_APP_NDEBUG
#define serialization_check_offset(cont)
#else
#define serialization_check_offset(cont)        \
    if (cont.offset == static_cast<size_t>(-1)) \
        return cont;
#endif

#define LN_SERIALIZER_VERSION "1001"

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
using serialization_container_data_return_type = typename serialization_container_func_return_type<
    serialization_container_has_data<T>::value && serialization_container_has_size<T>::value,
    T>::type1;

template <class T>
using serialization_container_size_return_type = typename serialization_container_func_return_type<
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

enum serialization_options : std::uint8_t
{
    LN_BINARY    = LN_BIT_MASK(0), // 二进制
    LN_HEX       = LN_BIT_MASK(1), // hex字符串
    LN_JSON      = LN_BIT_MASK(2), // json字符串
    LN_NO_HEADER = LN_BIT_MASK(3), // 无数据头
    LN_COMPACTED = LN_BIT_MASK(4), // 开启数据压缩
    LN_MEM       = LN_BIT_MASK(5), // 输出到内存
    LN_FILE      = LN_BIT_MASK(6), // 输出到文件
};

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
        if constexpr (Container::ops & serialization_options::LN_COMPACTED)
        {
            if constexpr (std::is_same<T, bool>::value
                          || cur_type_size == 1 || cur_type_size == 2
                          || std::is_same<T, float32_t>::value
                          || std::is_same<T, float64_t>::value)
            {
                cont.append(&value, cur_type_size);
                return cont;
            }
            else
            {
                uint8_t byte_size = need_byte_size(value);
                if (byte_size == cur_type_size)
                {
                    size_t temp = (static_cast<size_t>(value) << len_control_bit) | byte_size;
                    cont.append(&temp, byte_size);
                    uint8_t byte = static_cast<uint8_t>(static_cast<size_t>(value) >> (8 * cur_type_size - len_control_bit) & 0xF);
                    cont.append(&byte, 1);
                    return cont;
                }

                size_t temp = (static_cast<size_t>(value) << len_control_bit) | byte_size;
                cont.append(&temp, byte_size + 1);
                return cont;
            }
        }
        else
        {
            cont.append(&value, cur_type_size);
            return cont;
        }
    }

    template <class Container, class T, class = std::enable_if_t<is_basic_type_v<T>>>
    inline static Container& deserialize(Container& cont, T& value)
    {
        serialization_check_offset(cont);
        static constexpr uint8_t cur_type_size = sizeof(T);
        if constexpr (Container::ops & serialization_options::LN_COMPACTED)
        {
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
        else
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
        return (cont << value.size()).append(value.data(), value.size());
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
        if constexpr (N == 0) return cont;
        if constexpr (((Container::ops & serialization_options::LN_COMPACTED) == 0) && std::is_trivially_copyable_v<T>)
        {
            cont.append(&value, sizeof(T) * N);
            return cont;
        }
        else
        {
            for (auto& v : value) cont << v;
            return cont;
        }
    }

    template <class Container, class T, size_t N>
    inline static Container& deserialize(Container& cont, std::array<T, N>& value)
    {
        serialization_check_offset(cont);
        if constexpr (N == 0) return cont;
        if constexpr (((Container::ops & serialization_options::LN_COMPACTED) == 0) && std::is_trivially_copyable_v<T>)
        {
            if (cont.size() < cont.offset + sizeof(T) * N)
            {
                cont.offset = static_cast<size_t>(-1);
                return cont;
            }
            ::memcpy(&value, cont.cur(), sizeof(T) * N);
            cont.offset += sizeof(T) * N;
            return cont;
        }
        else
        {
            for (auto& v : value)
            {
                cont >> v;
                serialization_check_offset(cont);
            }
            return cont;
        }
    }

    // std::vector
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::vector<T>& value)
    {
        if constexpr (((Container::ops & serialization_options::LN_COMPACTED) == 0) && std::is_trivially_copyable_v<T>)
        {
            (cont << value.size()).append(value.data(), value.size() * sizeof(T));
            return cont;
        }
        else
        {
            cont << value.size();
            for (auto& v : value) cont << v;
            return cont;
        }
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::vector<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0) return cont;
        value.resize(size);
        if constexpr (((Container::ops & serialization_options::LN_COMPACTED) == 0) && std::is_trivially_copyable_v<T>)
        {
            size_t cur_type_size = sizeof(T) * size;
            if (cont.size() < cont.offset + cur_type_size)
            {
                cont.offset = static_cast<size_t>(-1);
                return cont;
            }
            ::memcpy(value.data(), cont.cur(), cur_type_size);
            cont.offset += cur_type_size;
            return cont;
        }
        else
        {
            for (auto& v : value)
            {
                cont >> v;
                serialization_check_offset(cont);
            }
        }

        return cont;
    }

    // std::deque
    template <class Container, class T>
    inline static Container& serialize(Container& cont, const std::deque<T>& value)
    {
        cont << value.size();
        for (auto& v : value) cont << v;
        return cont;
    }
    template <class Container, class T>
    inline static Container& deserialize(Container& cont, std::deque<T>& value)
    {
        serialization_check_offset(cont);
        size_t size = 0;
        cont >> size;
        serialization_check_offset(cont);
        if (size == 0) return cont;

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
    template <class T>
    static constexpr bool has_field_infos_v = std::tuple_size_v<decltype(::lntype_info_get_field_infos<T>())> > 0;

    // 拥有字段信息或是平凡可拷贝类型的
    template <class T>
    static constexpr bool is_serializable_object_v = has_field_infos_v<T> || std::is_trivially_copyable_v<T>;

    template <class Container, class T>
    struct serializable_object_proxy
    {
        static Container& serialize(Container& cont, const T& value)
        {
            // 开启数据压缩且可平凡复制优先通过直接拷贝
            if constexpr (((Container::ops & serialization_options::LN_COMPACTED) == 0) && std::is_trivially_copyable_v<T>)
            {
                cont.append(&value, sizeof(T));
                return cont;
            }
            else
            {
                // 申明字段信息，则优先通过字段信息序列化
                if constexpr (has_field_infos_v<T>)
                {
                    auto tuple = ::lntype_info_get_field_infos<T>();
                    return serialize_impl<Container, decltype(tuple), 0, T>(cont, tuple, value);
                }
                else
                {
                    cont.append(&value, sizeof(T));
                    return cont;
                }
            }
        }

        static Container& deserialize(Container& cont, T& value)
        {
            // 开启数据压缩且可平凡复制优先通过直接拷贝
            if constexpr (((Container::ops & serialization_options::LN_COMPACTED) == 0) && std::is_trivially_copyable_v<T>)
            {
                if (cont.size() < cont.offset + sizeof(T))
                {
                    cont.offset = static_cast<size_t>(-1);
                    return cont;
                }

                ::memcpy(&value, cont.cur(), sizeof(T));
                cont.offset += sizeof(T);
                return cont;
            }
            else
            {
                // 申明字段信息，则优先通过字段信息反序列化
                if constexpr (has_field_infos_v<T>)
                {
                    auto tuple = ::lntype_info_get_field_infos<T>();
                    return deserialize_impl<Container, decltype(tuple), 0, T>(cont, tuple, value);
                }
                else
                {
                    if (cont.size() < cont.offset + sizeof(T))
                    {
                        cont.offset = static_cast<size_t>(-1);
                        return cont;
                    }

                    ::memcpy(&value, cont.cur(), sizeof(T));
                    cont.offset += sizeof(T);
                    return cont;
                }
            }
        }

    private:
        template <class Container, class Tuple, size_t Idx, class T>
        inline static Container& serialize_impl(Container& cont, Tuple& tuple, const T& value)
        {
            if constexpr (Idx == std::tuple_size_v<Tuple>)
            {
                return cont;
            }
            else
            {
                cont << std::invoke(std::get<Idx>(tuple).second, value);
                return serialize_impl<Container, Tuple, Idx + 1>(cont, tuple, value);
            }
        }

        template <class Container, class Tuple, size_t Idx, class T>
        inline static Container& deserialize_impl(Container& cont, Tuple& tuple, T& value)
        {
            if constexpr (Idx == std::tuple_size_v<Tuple>)
            {
                return cont;
            }
            else
            {
                cont >> std::invoke(std::get<Idx>(tuple).second, value);
                return deserialize_impl<Container, Tuple, Idx + 1>(cont, tuple, value);
            }
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
        else if constexpr (serialization_serializable_object::is_serializable_object_v<T>)
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
        else if constexpr (serialization_serializable_object::is_serializable_object_v<T>)
        {
            serialization_serializable_object::serializable_object_proxy<Container, T>::deserialize(cont, value);
        }

        return cont;
    }

    template <class Container, class T, size_t N>
    inline static Container& serialize(Container& cont, const T (&value)[N])
    {
        if constexpr (N == 0) return cont;
        if constexpr (((Container::ops & serialization_options::LN_COMPACTED) == 0) && std::is_trivially_copyable_v<T>)
        {
            cont.append(&value, sizeof(T) * N);
            return cont;
        }
        else
        {
            for (auto& v : value) cont << v;
            return cont;
        }
    }
    template <class Container, class T, size_t N>
    inline static Container& deserialize(Container& cont, T (&value)[N])
    {
        serialization_check_offset(cont);
        if constexpr (N == 0) return cont;
        if constexpr (((Container::ops & serialization_options::LN_COMPACTED) == 0) && std::is_trivially_copyable_v<T>)
        {
            if (cont.size() < cont.offset + sizeof(T) * N)
            {
                cont.offset = static_cast<size_t>(-1);
                return cont;
            }
            ::memcpy(&value, cont.cur(), sizeof(T) * N);
            cont.offset += sizeof(T) * N;
            return cont;
        }
        else
        {
            for (auto& v : value)
            {
                cont >> v;
                serialization_check_offset(cont);
            }
            return cont;
        }
    }
};

template <class Container,
          uint8_t OPS = serialization_options::LN_BINARY | serialization_options::LN_NO_HEADER,
          class       = std::enable_if_t<serializer_internal::serialization_container_vaild<Container>>>
struct serialization_container
{
    Container             cont;
    size_t                offset = 0;
    static constexpr auto ops    = (OPS & serialization_options::LN_FILE) ? OPS : (OPS | serialization_options::LN_MEM);

    serialization_container& append(const void* data, size_t size)
    {
        using value_type = Container::value_type;
        cont.append(reinterpret_cast<const value_type*>(data), size);
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