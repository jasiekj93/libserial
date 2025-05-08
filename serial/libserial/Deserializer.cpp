#include "Deserializer.hpp"
#include <algorithm>

using namespace serial;

Deserializer::Deserializer()
    : level(MIN_LEVEL)
    , data()
    , indexes()
{
}

Deserializer::Deserializer(etl::span<const uint8_t> data)
    : Deserializer()
{
    deserialize(data);
}

Deserializer& Deserializer::deserialize(etl::span<const uint8_t> d)
{
    data = d;
    indexes.clear();

    if(data.empty())
        return *this;
    
    auto it = data.begin();
    indexes.push_back(0);

    while(true)
    {
        it = std::find(it, data.end(), level);
        if(it == data.end() or it == std::prev(data.end()))
            break;
        indexes.push_back(std::distance(data.begin(), std::next(it)));
        ++it;
    }

    return *this;
}

std::optional<std::string> Deserializer::getString(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    return std::string(data.begin() + start, data.begin() + end);
}

std::optional<char> Deserializer::getChar(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 1)
        return std::nullopt;

    return static_cast<char>(data[start]);
}

std::optional<uint8_t> Deserializer::getUint8(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 1)
        return std::nullopt;

    return static_cast<uint8_t>(data[start]);
}

std::optional<uint16_t> Deserializer::getUint16(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 2)
        return std::nullopt;

    return  static_cast<uint16_t>(data[start]) << 8 | 
            static_cast<uint16_t>(data[start + 1]);
}
std::optional<uint32_t> Deserializer::getUint32(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 4)
        return std::nullopt;

    return  static_cast<uint32_t>(data[start]) << 24 | 
            static_cast<uint32_t>(data[start + 1]) << 16 | 
            static_cast<uint32_t>(data[start + 2]) << 8 | 
            static_cast<uint32_t>(data[start + 3]);
}
std::optional<uint64_t> Deserializer::getUint64(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 8)
        return std::nullopt;

    return  static_cast<uint64_t>(data[start]) << 56 | 
            static_cast<uint64_t>(data[start + 1]) << 48 | 
            static_cast<uint64_t>(data[start + 2]) << 40 | 
            static_cast<uint64_t>(data[start + 3]) << 32 |
            static_cast<uint64_t>(data[start + 4]) << 24 | 
            static_cast<uint64_t>(data[start + 5]) << 16 | 
            static_cast<uint64_t>(data[start + 6]) << 8 | 
            static_cast<uint64_t>(data[start + 7]);
}
std::optional<uint8_t> Deserializer::getAsciiUint8(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 2)
        return std::nullopt;
    
    char buffer[3] = {0};
    std::copy(data.begin() + start, data.begin() + end, buffer);
    buffer[2] = '\0';

    return static_cast<uint8_t>(strtoul(buffer, nullptr, 16));
}
std::optional<uint16_t> Deserializer::getAsciiUint16(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 4)
        return std::nullopt;

    char buffer[5] = {0};
    std::copy(data.begin() + start, data.begin() + end, buffer);
    buffer[4] = '\0';

    return static_cast<uint16_t>(strtoul(buffer, nullptr, 16));
}
std::optional<uint32_t> Deserializer::getAsciiUint32(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 8)
        return std::nullopt;

    char buffer[9] = {0};
    std::copy(data.begin() + start, data.begin() + end, buffer);
    buffer[8] = '\0';

    return static_cast<uint32_t>(strtoul(buffer, nullptr, 16));
}
std::optional<uint64_t> Deserializer::getAsciiUint64(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 16)
        return std::nullopt;

    char buffer[17] = {0};
    std::copy(data.begin() + start, data.begin() + end, buffer);
    buffer[16] = '\0';

    return static_cast<uint64_t>(strtoull(buffer, nullptr, 16));
}
std::optional<bool> Deserializer::getAsciiBool(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    if(end - start != 1)
        return std::nullopt;

    return static_cast<bool>(data[start] - '0');
}

std::optional<etl::span<const uint8_t>> Deserializer::getData(int index) const
{
    if(index < 0 or index >= indexes.size())
        return std::nullopt;

    auto start = indexes[index];
    auto end = (index + 1 < indexes.size()) ? (indexes[index + 1] - 1) : data.size();

    return etl::span<const uint8_t>(data.begin() + start, data.begin() + end);
}

Deserializer& Deserializer::incrementLevel()
{
    if(level < MAX_LEVEL)
        ++level;

    return *this;
}

Deserializer& Deserializer::decrementLevel()
{
    if(level > MIN_LEVEL)
        --level;
    
    return *this;
}
