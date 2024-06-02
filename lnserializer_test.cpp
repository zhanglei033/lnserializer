#include "lnserializer.h"
#include "gtest/gtest.h"

using namespace ln;
using binary_t = serialization_container<std::basic_string<uint8_t>>;

TEST(lnserializer, test_bool)
{
    bool     serialize_value1 = true;
    bool     serialize_value2 = false;
    binary_t bin;
    {
        bin << serialize_value1;
        bin << serialize_value2;
    }

    {
        bool deserialize_value1;
        bool deserialize_value2;
        bin >> deserialize_value1;
        bin >> deserialize_value2;
        EXPECT_EQ(deserialize_value1, serialize_value1);
        EXPECT_EQ(deserialize_value2, serialize_value2);
    }
}

TEST(lnserializer, test_int8_t)
{
    binary_t bin;
    for (size_t i = 0; i < 255; i++)
    {
        bin << static_cast<int8_t>(i);
    }

    for (size_t i = 0; i < 255; i++)
    {
        int8_t i8;
        bin >> i8;
        EXPECT_EQ(i8, static_cast<int8_t>(i));
    }
}

TEST(lnserializer, test_uint8_t)
{
    binary_t bin;
    for (size_t i = 0; i < 255; i++)
    {
        bin << static_cast<uint8_t>(i);
    }

    for (size_t i = 0; i < 255; i++)
    {
        uint8_t u8;
        bin >> u8;
        EXPECT_EQ(u8, static_cast<uint8_t>(i));
    }
}

TEST(lnserializer, test_int16_t)
{
    binary_t bin;
    for (size_t i = 0; i < 65535; i++)
    {
        bin << static_cast<int16_t>(i);
    }

    for (size_t i = 0; i < 65535; i++)
    {
        int16_t i16;
        bin >> i16;
        EXPECT_EQ(i16, static_cast<int16_t>(i));
    }
}

TEST(lnserializer, test_uint16_t)
{
    binary_t bin;
    for (size_t i = 0; i < 65535; i++)
    {
        bin << static_cast<uint16_t>(i);
    }

    for (size_t i = 0; i < 65535; i++)
    {
        uint16_t u16;
        bin >> u16;
        EXPECT_EQ(u16, static_cast<uint16_t>(i));
    }
}

TEST(lnserializer, test_int32_t)
{
    binary_t bin;
    for (size_t i = 0; i < 0xFF; i++) bin << static_cast<int32_t>(i);
    for (size_t i = 0xFF00; i < 0xFFFF; i++) bin << static_cast<int32_t>(i);
    for (size_t i = 0xFFFF00; i < 0xFFFFFF; i++) bin << static_cast<int32_t>(i);
    for (size_t i = 0xFFFFFF00; i < 0xFFFFFFFF; i++) bin << static_cast<int32_t>(i);

    for (size_t i = 0; i < 0xFF; i++)
    {
        int32_t i32;
        bin >> i32;
        EXPECT_EQ(i32, static_cast<int32_t>(i));
    }

    for (size_t i = 0xFF00; i < 0xFFFF; i++)
    {
        int32_t i32;
        bin >> i32;
        EXPECT_EQ(i32, static_cast<int32_t>(i));
    }

    for (size_t i = 0xFFFF00; i < 0xFFFFFF; i++)
    {
        int32_t i32;
        bin >> i32;
        EXPECT_EQ(i32, static_cast<int32_t>(i));
    }

    for (size_t i = 0xFFFFFF00; i < 0xFFFFFFFF; i++)
    {
        int32_t i32;
        bin >> i32;
        EXPECT_EQ(i32, static_cast<int32_t>(i));
    }
}

TEST(lnserializer, test_uint32_t)
{
    binary_t bin;
    for (size_t i = 0; i < 0xFF; i++) bin << static_cast<uint32_t>(i);
    for (size_t i = 0xFF00; i < 0xFFFF; i++) bin << static_cast<uint32_t>(i);
    for (size_t i = 0xFFFF00; i < 0xFFFFFF; i++) bin << static_cast<uint32_t>(i);
    for (size_t i = 0xFFFFFF00; i < 0xFFFFFFFF; i++) bin << static_cast<uint32_t>(i);

    for (size_t i = 0; i < 0xFF; i++)
    {
        uint32_t u32;
        bin >> u32;
        EXPECT_EQ(u32, static_cast<uint32_t>(i));
    }

    for (size_t i = 0xFF00; i < 0xFFFF; i++)
    {
        uint32_t u32;
        bin >> u32;
        EXPECT_EQ(u32, static_cast<uint32_t>(i));
    }

    for (size_t i = 0xFFFF00; i < 0xFFFFFF; i++)
    {
        uint32_t u32;
        bin >> u32;
        EXPECT_EQ(u32, static_cast<uint32_t>(i));
    }

    for (size_t i = 0xFFFFFF00; i < 0xFFFFFFFF; i++)
    {
        uint32_t u32;
        bin >> u32;
        EXPECT_EQ(u32, static_cast<uint32_t>(i));
    }
}

TEST(lnserializer, test_int64_t)
{
    binary_t bin;
    for (size_t i = 0; i < 0xFF; i++) bin << static_cast<int64_t>(i);
    for (size_t i = 0xFF00; i < 0xFFFF; i++) bin << static_cast<int64_t>(i);
    for (size_t i = 0xFFFF00; i < 0xFFFFFF; i++) bin << static_cast<int64_t>(i);
    for (size_t i = 0xFFFFFF00; i < 0xFFFFFFFF; i++) bin << static_cast<int64_t>(i);
    for (size_t i = 0xFFFFFFFF00; i < 0xFFFFFFFFFF; i++) bin << static_cast<int64_t>(i);
    for (size_t i = 0xFFFFFFFFFF00; i < 0xFFFFFFFFFFFF; i++) bin << static_cast<int64_t>(i);
    for (size_t i = 0xFFFFFFFFFFFF00; i < 0xFFFFFFFFFFFFFF; i++) bin << static_cast<int64_t>(i);
    for (size_t i = 0xFFFFFFFFFFFFFF00; i < 0xFFFFFFFFFFFFFFFF; i++) bin << static_cast<int64_t>(i);

    for (size_t i = 0; i < 0xFF; i++)
    {
        int64_t i64;
        bin >> i64;
        EXPECT_EQ(i64, static_cast<int64_t>(i));
    }

    for (size_t i = 0xFF00; i < 0xFFFF; i++)
    {
        int64_t i64;
        bin >> i64;
        EXPECT_EQ(i64, static_cast<int64_t>(i));
    }

    for (int64_t i = 0xFFFF00; i < 0xFFFFFF; i++)
    {
        int64_t i64;
        bin >> i64;
        EXPECT_EQ(i64, static_cast<int64_t>(i));
    }

    for (size_t i = 0xFFFFFF00; i < 0xFFFFFFFF; i++)
    {
        int64_t i64;
        bin >> i64;
        EXPECT_EQ(i64, static_cast<int64_t>(i));
    }

    for (size_t i = 0xFFFFFFFF00; i < 0xFFFFFFFFFF; i++)
    {
        int64_t i64;
        bin >> i64;
        EXPECT_EQ(i64, static_cast<int64_t>(i));
    }
    for (size_t i = 0xFFFFFFFFFF00; i < 0xFFFFFFFFFFFF; i++)
    {
        int64_t i64;
        bin >> i64;
        EXPECT_EQ(i64, static_cast<int64_t>(i));
    }
    for (size_t i = 0xFFFFFFFFFFFF00; i < 0xFFFFFFFFFFFFFF; i++)
    {
        int64_t i64;
        bin >> i64;
        EXPECT_EQ(i64, static_cast<int64_t>(i));
    }
    for (size_t i = 0xFFFFFFFFFFFFFF00; i < 0xFFFFFFFFFFFFFFFF; i++)
    {
        int64_t i64;
        bin >> i64;
        EXPECT_EQ(i64, static_cast<int64_t>(i));
    }
}

TEST(lnserializer, test_uint64_t)
{
    binary_t bin;
    for (size_t i = 0; i < 0xFF; i++) bin << static_cast<uint64_t>(i);
    for (size_t i = 0xFF00; i < 0xFFFF; i++) bin << static_cast<uint64_t>(i);
    for (size_t i = 0xFFFF00; i < 0xFFFFFF; i++) bin << static_cast<uint64_t>(i);
    for (size_t i = 0xFFFFFF00; i < 0xFFFFFFFF; i++) bin << static_cast<uint64_t>(i);
    for (size_t i = 0xFFFFFFFF00; i < 0xFFFFFFFFFF; i++) bin << static_cast<uint64_t>(i);
    for (size_t i = 0xFFFFFFFFFF00; i < 0xFFFFFFFFFFFF; i++) bin << static_cast<uint64_t>(i);
    for (size_t i = 0xFFFFFFFFFFFF00; i < 0xFFFFFFFFFFFFFF; i++) bin << static_cast<uint64_t>(i);
    for (size_t i = 0xFFFFFFFFFFFFFF00; i < 0xFFFFFFFFFFFFFFFF; i++) bin << static_cast<uint64_t>(i);

    for (size_t i = 0; i < 0xFF; i++)
    {
        uint64_t u64;
        bin >> u64;
        EXPECT_EQ(u64, static_cast<uint64_t>(i));
    }

    for (size_t i = 0xFF00; i < 0xFFFF; i++)
    {
        uint64_t u64;
        bin >> u64;
        EXPECT_EQ(u64, static_cast<uint64_t>(i));
    }

    for (uint64_t i = 0xFFFF00; i < 0xFFFFFF; i++)
    {
        uint64_t u64;
        bin >> u64;
        EXPECT_EQ(u64, static_cast<uint64_t>(i));
    }

    for (size_t i = 0xFFFFFF00; i < 0xFFFFFFFF; i++)
    {
        uint64_t u64;
        bin >> u64;
        EXPECT_EQ(u64, static_cast<uint64_t>(i));
    }

    for (size_t i = 0xFFFFFFFF00; i < 0xFFFFFFFFFF; i++)
    {
        uint64_t u64;
        bin >> u64;
        EXPECT_EQ(u64, static_cast<uint64_t>(i));
    }
    for (size_t i = 0xFFFFFFFFFF00; i < 0xFFFFFFFFFFFF; i++)
    {
        uint64_t u64;
        bin >> u64;
        EXPECT_EQ(u64, static_cast<uint64_t>(i));
    }
    for (size_t i = 0xFFFFFFFFFFFF00; i < 0xFFFFFFFFFFFFFF; i++)
    {
        uint64_t u64;
        bin >> u64;
        EXPECT_EQ(u64, static_cast<uint64_t>(i));
    }
    for (size_t i = 0xFFFFFFFFFFFFFF00; i < 0xFFFFFFFFFFFFFFFF; i++)
    {
        uint64_t u64;
        bin >> u64;
        EXPECT_EQ(u64, static_cast<uint64_t>(i));
    }
}

TEST(lnserializer, test_float32_t)
{
    constexpr auto min = std::numeric_limits<float32_t>::min();
    constexpr auto max = std::numeric_limits<float32_t>::max();
    binary_t       bin;
    for (auto value = min; value < max; value *= 10.0)
    {
        bin << value;
    }

    for (auto value = min; value < max; value *= 10.0)
    {
        float32_t dvalue;
        bin >> dvalue;
        EXPECT_EQ(dvalue, value);
    }
}

TEST(lnserializer, test_float64_t)
{
    constexpr auto min = std::numeric_limits<float64_t>::min();
    constexpr auto max = std::numeric_limits<float64_t>::max();
    binary_t       bin;
    for (auto value = min; value < max; value *= 10.0)
    {
        bin << value;
    }

    for (auto value = min; value < max; value *= 10.0)
    {
        float64_t dvalue;
        bin >> dvalue;
        EXPECT_EQ(dvalue, value);
    }
}

TEST(lnserializer, test_arr)
{
    binary_t bin;
    int8_t   i8s[]  = {1, 5, 8, 66, 88, 124, 235};
    uint8_t  u8s[]  = {1, 5, 8, 66, 88, 124, 235};
    int16_t  i16s[] = {0x5, 0x75, 0xF5, 0x7F5, 0xFF5, 0x7FF5, 0xFFF5};
    uint16_t u16s[] = {0x5, 0x75, 0xF5, 0x7F5, 0xFF5, 0x7FF5, 0xFFF5};
    int16_t  i32s[] = {0xF, 0xFFF, 0xFFFFF, 0xFFFF};
    uint16_t u32s[] = {0xF, 0xFFF, 0xFFFFF, 0xFFFF};
    int16_t  i64s[] = {0xFF, 0xFFF, 0xFFFF, 0xFFFFF, 0xFFFFF, 0xFFFFF};
    uint16_t u64s[] = {0xFF, 0xFFF, 0xFFFF, 0xFFFFF, 0xFFFFF, 0xFFFFF};
    bin << i8s
        << u8s
        << i16s
        << u16s
        << i32s
        << u32s
        << i64s
        << u64s;
    int8_t   di8s[7];
    uint8_t  du8s[7];
    int16_t  di16s[7];
    uint16_t du16s[7];
    int16_t  di32s[4];
    uint16_t du32s[4];
    int16_t  di64s[6];
    uint16_t du64s[6];
    bin >> di8s
        >> du8s
        >> di16s
        >> du16s
        >> di32s
        >> du32s
        >> di64s
        >> du64s;
    for (int i = 0; i < 7; i++) EXPECT_EQ(i8s[i], di8s[i]);
    for (int i = 0; i < 7; i++) EXPECT_EQ(u8s[i], du8s[i]);
    for (int i = 0; i < 7; i++) EXPECT_EQ(i16s[i], di16s[i]);
    for (int i = 0; i < 7; i++) EXPECT_EQ(u16s[i], du16s[i]);
    for (int i = 0; i < 4; i++) EXPECT_EQ(i32s[i], di32s[i]);
    for (int i = 0; i < 4; i++) EXPECT_EQ(u32s[i], du32s[i]);
    for (int i = 0; i < 6; i++) EXPECT_EQ(i64s[i], di64s[i]);
    for (int i = 0; i < 6; i++) EXPECT_EQ(u64s[i], du64s[i]);
}

TEST(lnserializer, test_stdstring)
{
    binary_t    bin;
    std::string str("asiodjlkajskdjakjsdkjasjdpoaj;lsdj;");
    bin << str;
    std::string destr;
    bin >> destr;
    EXPECT_EQ(true, destr == str);
}

TEST(lnserializer, test_stdarray)
{
    binary_t            bin;
    std::array<int, 10> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    bin << arr;
    std::array<int, 10> dearr;
    bin >> dearr;
    EXPECT_EQ(true, dearr == arr);
}

TEST(lnserializer, test_stdvector)
{
    binary_t         bin;
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    bin << vec;
    std::vector<int> devec;
    bin >> devec;
    EXPECT_EQ(true, devec == vec);
}

TEST(lnserializer, test_stddeque)
{
    binary_t        bin;
    std::deque<int> deq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    bin << deq;
    std::deque<int> dedeq;
    bin >> dedeq;
    EXPECT_EQ(true, dedeq == deq);
}

TEST(lnserializer, test_stdlist)
{
    binary_t       bin;
    std::list<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    bin << v1;
    std::list<int> v2;
    bin >> v2;
    EXPECT_EQ(true, v1 == v2);
}

TEST(lnserializer, test_stdmap)
{
    binary_t           bin;
    std::map<int, int> v1 = {{1, 2}, {2, 3}, {3, 4}, {5, 6}, {7, 8}};
    bin << v1;
    std::map<int, int> v2;
    bin >> v2;
    EXPECT_EQ(true, v1 == v2);
}

class custom_t
{
public:
    uint8_t     u8;
    uint16_t    u16;
    float32_t   f32;
    float64_t   f64;
    std::string str;

    bool operator==(const custom_t& other)
    {
        return u8 == other.u8 && u16 == other.u16
               && f32 == other.f32 && f64 == other.f64 && str == other.str;
    }

    auto deserialize_tuple()
    {
        std::tuple<uint8_t&,
                   uint16_t&,
                   float32_t&,
                   float64_t&,
                   std::string&>
            tuple(u8, u16, f32, f64, str);
        return tuple;
    }

    auto serialize_tuple() const
    {
        std::tuple<const uint8_t&,
                   const uint16_t&,
                   const float32_t&,
                   const float64_t&,
                   const std::string&>
            tuple(u8, u16, f32, f64, str);
        return tuple;
    }
};
TEST(lnserializer, test_custom)
{
    binary_t bin;
    custom_t v1;
    v1.u8  = 0xff;
    v1.u16 = 0xff;
    v1.f32 = 12.34;
    v1.f64 = 123.345;
    v1.str.assign("sakjdlkajskljdlkajskldjlkasjlkdjalkjdlkasjslkdjl");
    bin << v1;
    custom_t v2;
    bin >> v2;
    EXPECT_EQ(true, v1 == v2);
}
 int main(int argc, char** argv)
 {
     testing::InitGoogleTest(&argc, argv);
     return ::testing::UnitTest::GetInstance()->Run();
 }