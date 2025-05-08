#pragma once

/**
 * @file Frame.hpp
 * @author Adrian Szczepanski
 * @date 08-05-2025
 */

#include <optional>
#include <utility>

#include <libserial/Serializer.hpp>
#include <libserial/Crc.hpp>

namespace serial
{
    /** Frame structure:
     * ------------------------------------------------------------------------------------------------------------
     * | Start Flag | Type       | Payload Start | Payload Data          | Payload End | CRC           | End Flag |
     * ---------------------------------------------------------------------------------------------------
     * | 0x01       | X (1 byte) | 0x02          | (n bytes) - no flags! | 0x03        | (2 bytes)     | 0x04     |
     * ------------------------------------------------------------------------------------------------------------
     * @note The payload data should not contain any flags (0x01, 0x02, 0x03, 0x04). Should only be ASCII printable chars.
     * @note Use Serializer to serialize the payload data.
     */
    class Frame
    {
    public:
        static constexpr uint8_t START_FLAG = 0x01;
        static constexpr uint8_t END_FLAG = 0x04;
        static constexpr uint8_t PAYLOAD_START = 0x02;
        static constexpr uint8_t PAYLOAD_END = 0x03;

        static constexpr uint8_t MIN_SIZE = 7; // Start flag + Type + Payload start + Payload End + CRC + End flag

        using Type = uint8_t;

        static std::pair<std::optional<Frame>, etl::span<const uint8_t>::iterator> find(etl::span<const uint8_t> data);

        Frame(Type, etl::span<const uint8_t> data);

        Serializer serialize();

        inline auto data() const { return payload; }
        inline auto getType() const { return type; }

        bool operator==(const Frame& other) const
        {
            return (type == other.type and 
                etl::equal(payload, other.payload));
        }

    protected:
        static std::pair<std::optional<Frame>, etl::span<const uint8_t>::iterator> findInBuffer(etl::span<const uint8_t> data);
        static std::optional<Frame> deserialize(etl::span<const uint8_t> data);

    private:
        Type type;
        etl::span<const uint8_t> payload;
    };
}