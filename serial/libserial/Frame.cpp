#include "Frame.hpp"

using namespace serial;

std::pair<std::optional<Frame>, etl::span<const uint8_t>::iterator> Frame::find(etl::span<const uint8_t> data)
{
    auto begin = data.begin();

    while(true)
    {
        auto start = std::find(begin, data.end(), START_FLAG);

        if(start == data.end())
            return { std::nullopt, data.end() };
        
        auto [result, iterator] = findInBuffer(etl::span<const uint8_t>{start, data.end()});
        if(result.has_value())
            return { result, iterator };
        else
        {
            if(iterator == data.end())
                return { std::nullopt, data.end() };
            else
                begin = std::next(iterator);
        }
    }
}

Frame::Frame(Type type, etl::span<const uint8_t> data)
    : type(type)
    , payload(data)
{
}

Serializer Frame::serialize()
{
    Serializer serializer;

    serializer
        .number(START_FLAG)
        .number(type)
        .number(PAYLOAD_START)
        .data(payload)
        .number(PAYLOAD_END)
        .number(Crc::calculate(payload))
        .number(END_FLAG);

    return serializer;
}

std::pair<std::optional<Frame>, etl::span<const uint8_t>::iterator> Frame::findInBuffer(etl::span<const uint8_t> data)
{
    if(data.size() < MIN_SIZE)
        return { std::nullopt, data.end() };

    auto start = data.begin();
    auto payloadStart = std::next(start, 2);

    if(payloadStart >= data.end() or 
        *payloadStart != PAYLOAD_START)
        return { std::nullopt, payloadStart };

    auto payloadEnd = std::find(payloadStart, data.end(), PAYLOAD_END);
    if (payloadEnd == data.end())
        return { std::nullopt, payloadEnd };

    auto end = std::find(payloadEnd, data.end(), END_FLAG);
    if (end == data.end())
        return { std::nullopt, end };

    // CRC check
    if(std::distance(payloadEnd, end) != 3)
        return { std::nullopt, std::next(end) };
    
    auto crcIt = std::next(payloadEnd, 1);
    auto crc = (static_cast<uint16_t>(*(crcIt)) << 8) | *(std::next(crcIt, 1));

    if(Crc::validate(etl::span<const uint8_t>{std::next(payloadStart), payloadEnd}, crc) == false)
        return { std::nullopt, std::next(end) };

    return { deserialize({start, std::next(end)}), std::next(end) };
}

std::optional<Frame> Frame::deserialize(etl::span<const uint8_t> data)
{
    auto type = data[1];

    auto payloadStart = std::next(data.begin(), 3);
    auto payloadEnd = std::find(payloadStart, data.end(), PAYLOAD_END);

    if(payloadEnd == data.end())
        return std::nullopt;

    return Frame(type, etl::span<const uint8_t>{payloadStart, payloadEnd});
}

