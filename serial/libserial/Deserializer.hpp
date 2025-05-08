#pragma once

/**
 * @file Serializer.hpp
 * @author Adrian Szczepanski
 * @date 2025-05-06
 */

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

#include <etl/span.h>

namespace serial
{
    class Deserializer
    {
    public:
        Deserializer(etl::span<const uint8_t> data, uint8_t delimiter = '\0');

        inline auto count() const { return indexes.size(); }

        std::optional<std::string> getString(int) const;
        std::optional<char> getChar(int) const;

        std::optional<uint8_t> getUint8(int) const;
        std::optional<uint16_t> getUint16(int) const;
        std::optional<uint32_t> getUint32(int) const;
        std::optional<uint64_t> getUint64(int) const;

        std::optional<uint8_t> getAsciiUint8(int) const;
        std::optional<uint16_t> getAsciiUint16(int) const;
        std::optional<uint32_t> getAsciiUint32(int) const;
        std::optional<uint64_t> getAsciiUint64(int) const;
        std::optional<bool> getAsciiBool(int) const;

        etl::span<const uint8_t> getData(int) const;


    protected:

    private:
        etl::span<const uint8_t> data;
        std::vector<int> indexes;
    };
}
