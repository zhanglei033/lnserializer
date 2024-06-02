#pragma once
#include <string>
#include <vector>

#include <stdint.h>

#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/mem_streams.hpp>
#include <yas/std_types.hpp>

namespace yas_test {

struct Record
{
    using integers_t = std::vector<int64_t>;
    using strings_t  = std::vector<std::string>;

    integers_t ids;
    strings_t  strings;

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar & ids & strings;
    }

    template <std::size_t opts>
    void to_string(std::string& data)
    {
        yas::mem_ostream                             os;
        yas::binary_oarchive<yas::mem_ostream, opts> oa(os);
        oa&* this;

        auto buf = os.get_intrusive_buffer();
        data.assign(buf.data, buf.size);
    }

    template <std::size_t opts>
    void from_string(const std::string& data)
    {
        yas::mem_istream                             is(data.c_str(), data.size());
        yas::binary_iarchive<yas::mem_istream, opts> ia(is);
        ia&* this;
    }
};

} // namespace yas_test
