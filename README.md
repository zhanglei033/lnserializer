# lnserializer
C++序列化库

序列化1024条字符串，序列化和反序列化1000次 
serializer          |version|count|size   |time
--------------------|-------|-----|-------|----
protobuf            |5027000|10000|1469032|286
lnserializer        |1001   |10000|1474239|110
lnserializer-compact|1001   |10000|1468069|112
yas                 |7.2.0  |10000|1474239|1427
yas-compact         |7.2.0  |10000|1468916|1239
cereal              |1.3.2  |10000|1474239|2466

序列化859个整数，序列化和反序列化100000次 
serializer          |version|count |size |time
--------------------|-------|------|-----|----
protobuf            |5027000|100000|4573 |540
lnserializer        |1001   |100000|6896 |13
lnserializer-compact|1001   |100000|4495 |897
yas                 |7.2.0  |100000|6896 |29
yas-compact         |7.2.0  |100000|4857 |693
cereal              |1.3.2  |100000|6896 |207


序列化3498个浮点数，序列化和反序列化100000次 
serializer          |version|count |size |time
--------------------|-------|------|-----|----
protobuf            |5027000|100000|27988|108
lnserializer        |1001   |100000|28008|119
lnserializer-compact|1001   |100000|27988|120
yas                 |7.2.0  |100000|28008|249
yas-compact         |7.2.0  |100000|27989|703
cereal              |1.3.2  |100000|28008|455

```CPP
// 简单示例
#include "lnserializer.h"
using namespace ln;
using binary_t = serialization_container<std::basic_string<uint8_t>>;
binary_t bin;
int v1 = 12345,v2 = 0; // v1 = 12345 v2 = 0
bin << v1;
bin >> v2; // v2 = 12345

// ------
binary_t bin;
std::vector<int> v1 = {12345,23456,34567,45789,56789};
std::vector<int> v2;
bin << v1;
bin >> v2; // v2 = {12345,23456,34567,45789,56789};

// ------
binary_t bin;
std::vector<int> v1 = {12345,23456,34567,45789,56789};
std::vector<int> v2;
bin << v1;
bin >> v2; // v2 = {12345,23456,34567,45789,56789};

// ------
class custom_data_t
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

binary_t bin;
custom_t v1;
v1.u8  = 0xff;
v1.u16 = 0xff;
v1.f32 = 12.34;
v1.f64 = 123.345;
v1.str.assign("sakjdlkajskljdlkajskldjlkasjlkdjalkjdlkasjslkdjl");
custom_t v2;
bin << v1;
bin >> v2;

```

