#include "cereal/record.h"
#include "lnutility.h"
#include "protobuf/test.pb.h"
#include "yas/record.h"
#include <string>

#define LN_PP_STRING_IMPL(x) #x
#define LN_PP_STRING(x)      LN_PP_STRING_IMPL(x)

struct benchmark_data
{
    std::vector<uint64_t>     ints;
    std::vector<float64_t>   floats;
    std::vector<std::string> strs;
    uint64_t                 count;
};

struct benchmark_result
{
    benchmark_result(std::string name, std::string version, size_t size, uint64_t time)
        : name(name), version(version), size(size), time(time)
    {
    }

    std::string name;
    std::string version;
    size_t      size = 0;
    uint64_t    time = 0;
};

enum class benchmark_type_e : int
{
    LN_STRING,  // 字符串
    LN_INTEGER, // 整数
    LN_FLOAT,   // 浮点数
    LN_ALL,     //
};

void benchmark_data_init(benchmark_data& data, benchmark_type_e type)
{
    if (type == benchmark_type_e::LN_STRING || type == benchmark_type_e::LN_ALL)
    {
        std::string str;
        for (int i = 1; i < 1024; i++)
        {
            str.append(std::to_string(i));
            data.strs.push_back(str);
        }
    }

    if (type == benchmark_type_e::LN_INTEGER || type == benchmark_type_e::LN_ALL)
    {
        for (uint64_t i = std::numeric_limits<uint64_t>::max(); i > std::numeric_limits<uint64_t>::min(); i /= 1.05)
        {
            data.ints.push_back(i);
        }
    }

    if (type == benchmark_type_e::LN_FLOAT || type == benchmark_type_e::LN_ALL)
    {
        for (float64_t f = std::numeric_limits<float64_t>::min(); f < std::numeric_limits<float64_t>::max(); f *= 1.5)
        {
            data.floats.push_back(f);
        }
    }
}

benchmark_result benchmark_protobuf_serialization(benchmark_data& data)
{
    using namespace protobuf_test;
    Record r1;
    for (size_t i = 0; i < data.ints.size(); i++)
    {
        r1.add_ids(data.ints[i]);
    }
    for (size_t i = 0; i < data.strs.size(); i++)
    {
        r1.add_strings(data.strs[i]);
    }
    for (size_t i = 0; i < data.floats.size(); i++)
    {
        r1.add_floats(data.floats[i]);
    }

    std::string serialized;
    r1.SerializeToString(&serialized);
    Record r2;
    bool   ok = r2.ParseFromString(serialized);
    if (!ok)
    {
        throw std::logic_error("protobuf's case: deserialization failed");
    }
    for (size_t i = 0; i < data.ints.size(); i++)
    {
        if (r1.ids().Get(i) != r2.ids().Get(i))
            throw std::logic_error("protobuf's case: deserialization failed : ids");
    }
    for (size_t i = 0; i < data.strs.size(); i++)
    {
        if (r1.strings().Get(i) != r2.strings().Get(i))
            throw std::logic_error("protobuf's case: deserialization failed : strings");
    }
    for (size_t i = 0; i < data.floats.size(); i++)
    {
        if (r1.floats().Get(i) != r2.floats().Get(i))
            throw std::logic_error("protobuf's case: deserialization failed : floats");
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < data.count; i++)
    {
        serialized.clear();
        r1.SerializeToString(&serialized);
        r2.ParseFromString(serialized);
    }
    auto finish   = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    return benchmark_result("protobuf", LN_PP_STRING(GOOGLE_PROTOBUF_VERSION), serialized.size(), duration);
}

struct lnserializerRecord
{
    using integers_t = std::vector<uint64_t>;
    using floats_t   = std::vector<float64_t>;
    using strings_t  = std::vector<std::string>;

    integers_t ids;
    floats_t   floats;
    strings_t  strings;
};
LN_TYPE_INFO_FIELDS_DECL(lnserializerRecord, ids, floats, strings);

template <std::size_t opts>
benchmark_result benchmark_lnserializer_serialization(benchmark_data& data)
{
    using namespace ln;
    using container_t = serialization_container<std::string, opts>;

    lnserializerRecord r1;
    r1.ids     = data.ints;
    r1.strings = data.strs;
    r1.floats  = data.floats;

    container_t serialized;
    serialized << r1;
    lnserializerRecord r2;
    serialized >> r2;

    if (serialized.offset == static_cast<size_t>(-1))
        throw std::logic_error("lnserializer's case: deserialization failed");
    if (r1.ids != r2.ids)
        throw std::logic_error("lnserializer's case: deserialization failed : ids");
    if (r1.strings != r2.strings)
        throw std::logic_error("lnserializer's case: deserialization failed : strings");
    if (r1.floats != r2.floats)
        throw std::logic_error("lnserializer's case: deserialization failed : floats");

    std::string tag;
    if (opts & serialization_options::LN_COMPACTED)
    {
        tag = "lnserializer-compact";
    }
    else
    {
        tag = "lnserializer";
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < data.count; i++)
    {
        serialized.cont.clear();
        serialized.offset = 0;
        serialized << r1;
        serialized >> r2;
    }
    auto finish   = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    return benchmark_result(tag, LN_SERIALIZER_VERSION, serialized.size(), duration);
}

template <std::size_t opts>
benchmark_result benchmark_yas_serialization(benchmark_data& data)
{
    using namespace yas_test;
    Record r1;
    Record r2;
    r1.ids     = data.ints;
    r1.strings = data.strs;
    r1.floats  = data.floats;

    std::string serialized;

    r1.to_string<opts>(serialized);
    r2.from_string<opts>(serialized);

    if (r1.ids != r2.ids)
        throw std::logic_error("yas's case: deserialization failed : ids");
    if (r1.strings != r2.strings)
        throw std::logic_error("yas's case: deserialization failed : strings");
    if (r1.floats != r2.floats)
        throw std::logic_error("yas's case: deserialization failed : floats");

    std::string tag;
    if (opts & yas::compacted)
    {
        tag = "yas-compact";
    }
    else
    {
        tag = "yas";
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < data.count; i++)
    {
        serialized.clear();
        r1.to_string<opts>(serialized);
        r2.from_string<opts>(serialized);
    }
    auto finish   = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

    return benchmark_result(tag, YAS_VERSION_STRING, serialized.size(), duration);
}

benchmark_result benchmark_cereal_serialization(benchmark_data& data)
{
    using namespace cereal_test;

    Record r1;
    Record r2;
    r1.ids     = data.ints;
    r1.strings = data.strs;
    r1.floats  = data.floats;

    std::string serialized;

    r1.to_string(serialized);
    r2.from_string(serialized);

    if (r1 != r2)
    {
        throw std::logic_error("cereal's case: deserialization failed");
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < data.count; i++)
    {
        serialized.clear();
        r1.to_string(serialized);
        r2.from_string(serialized);
    }
    auto finish   = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

    return benchmark_result("cereal", "1.3.2", serialized.size(), duration);
}

//
int main(int argc, char** argv)
{
    int            benchmark_type = std::stoi(argv[1]);
    size_t         count          = std::stoull(argv[2]);
    benchmark_data data;
    benchmark_data_init(data, static_cast<benchmark_type_e>(benchmark_type));
    data.count = count;
    std::vector<benchmark_result> results;
    results.push_back(benchmark_protobuf_serialization(data));
    results.push_back(benchmark_lnserializer_serialization<
                      ln::serialization_options::LN_BINARY
                      | ln::serialization_options::LN_NO_HEADER>(data));
    results.push_back(benchmark_lnserializer_serialization<
                      ln::serialization_options::LN_BINARY
                      | ln::serialization_options::LN_NO_HEADER
                      | ln::serialization_options::LN_COMPACTED>(data));
    results.push_back(benchmark_yas_serialization<yas::binary | yas::no_header>(data));
    results.push_back(benchmark_yas_serialization<yas::binary | yas::no_header | yas::compacted>(data));
    results.push_back(benchmark_cereal_serialization(data));

    std::cout << "serializer\tversion\tcount\tsize\ttime" << std::endl;
    for (const auto& result : results)
    {
        std::cout << result.name << "\t" << result.version << "\t" << count << "\t" << result.size << "\t" << result.time << std::endl;
    }

    //for (const auto& result : results)
    //{
    //    std::cout << "Serializer: " << result.name << std::endl;
    //    std::cout << "Version   : " << result.version << std::endl;
    //    std::cout << "count     : " << count << std::endl;
    //    std::cout << "Size      : " << result.size << " bytes" << std::endl;
    //    std::cout << "Time      : " << result.time << " milliseconds" << std::endl;
    //    std::cout << std::endl;
    //}
    return 0;
}