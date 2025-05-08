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

#include <libserial/Serializable.hpp>

namespace serial
{
    class Deserializer
    {
    public:
        static constexpr uint8_t MAX_LEVEL = 255;
        static constexpr uint8_t MIN_LEVEL = 128;

        Deserializer();
        Deserializer(etl::span<const uint8_t> data);

        Deserializer& deserialize(etl::span<const uint8_t> data);

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

        std::optional<etl::span<const uint8_t>> getData(int) const;

        Deserializer& incrementLevel();
        Deserializer& decrementLevel();

    protected:

    private:
        uint8_t level = MIN_LEVEL;
        etl::span<const uint8_t> data;
        std::vector<int> indexes;
    };
}
