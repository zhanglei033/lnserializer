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

typedef std::vector<uint64_t>    Integers;
typedef std::vector<std::string> Strings;
typedef std::vector<double>      Floats;

class Record {
public:

    Integers ids;
    Strings  strings;
    Floats   floats;

    bool operator==(const Record &other) {
        return (ids == other.ids && strings == other.strings && floats == other.floats);
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
        archive(ids, strings, floats);
    }
};


} // namespace

#endif
