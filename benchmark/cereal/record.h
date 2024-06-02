#ifndef __CEREAL_RECORD_HPP_INCLUDED__
#define __CEREAL_RECORD_HPP_INCLUDED__

#include <vector>
#include <string>
#include <sstream>

#include <stdint.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace cereal_test {

typedef std::vector<int64_t>     Integers;
typedef std::vector<std::string> Strings;

class Record {
public:

    Integers ids;
    Strings  strings;

    bool operator==(const Record &other) {
        return (ids == other.ids && strings == other.strings);
    }

    bool operator!=(const Record &other) {
        return !(*this == other);
    }
    void to_string(std::string& data)
    {
        std::ostringstream          stream;
        cereal::BinaryOutputArchive archive(stream);
        archive(*this);
        data = stream.str();
    }

    void from_string(const std::string& data)
    {
        std::stringstream          stream(data);
        cereal::BinaryInputArchive archive(stream);
        archive(*this);
    }

private:

    friend class cereal::access;

    template<typename Archive>
    void serialize(Archive &archive)
    {
        archive(ids, strings);
    }
};


} // namespace

#endif
