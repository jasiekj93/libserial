#include "Serializer.hpp"
#include <cstring>

using namespace serial;

Serializer::Serializer()
{
}

Serializer& Serializer::delim()
{
    buffer.push_back(level);
    return *this;
}

Serializer& Serializer::put(char c)
{
    buffer.push_back(c);
    return *this;
}

Serializer& Serializer::ascii(uint8_t value)
{
    char array[4];
    snprintf(array, sizeof(array), "%02X", value);
    buffer.insert(buffer.end(), array, array + 2);
    return *this;
}

Serializer& Serializer::ascii(uint16_t value)
{
    char array[6];
    snprintf(array, sizeof(array), "%04X", value);
    buffer.insert(buffer.end(), array, array + 4);
    return *this;
}

Serializer& Serializer::ascii(uint32_t value)
{
    char array[10];
    snprintf(array, sizeof(array), "%08X", value);
    buffer.insert(buffer.end(), array, array + 8);
    return *this;
}

Serializer& Serializer::ascii(uint64_t value)
{
    char array[18];
    snprintf(array, sizeof(array), "%016llX", static_cast<unsigned long long>(value));
    buffer.insert(buffer.end(), array, array + 16);
    return *this;
}

Serializer& Serializer::ascii(bool value)
{
    buffer.push_back(value ? '1' : '0');
    return *this;
}

Serializer& Serializer::number(uint8_t value)
{
    buffer.push_back(value);
    return *this;
}

Serializer& Serializer::number(uint16_t value)
{
    buffer.push_back(static_cast<uint8_t>(value >> 8));
    buffer.push_back(static_cast<uint8_t>(value & 0xFF));
    return *this;
}

Serializer& Serializer::number(uint32_t value)
{
    buffer.push_back(static_cast<uint8_t>(value >> 24));
    buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    buffer.push_back(static_cast<uint8_t>(value & 0xFF));
    return *this;
}

Serializer& Serializer::number(uint64_t value)
{
    buffer.push_back(static_cast<uint8_t>(value >> 56));
    buffer.push_back(static_cast<uint8_t>((value >> 48) & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 40) & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 32) & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    buffer.push_back(static_cast<uint8_t>(value & 0xFF));
    return *this;
}

Serializer& Serializer::string(std::string_view str)
{
    buffer.insert(buffer.end(), str.begin(), str.end());
    return *this;
}

Serializer& Serializer::string(const char *str)
{
    buffer.insert(buffer.end(), str, str + std::strlen(str));
    return *this;
}

Serializer& Serializer::data(etl::span<const uint8_t> span)
{
    buffer.insert(buffer.end(), span.begin(), span.end());
    return *this;
}

Serializer& Serializer::object(const Serializable& object)
{
    incrementLevel();
    object.serialize(*this);
    decrementLevel();

    return *this;
}

Serializer& Serializer::incrementLevel()
{
    if(level < MAX_LEVEL)
        ++level;

    return *this;
}

Serializer& Serializer::decrementLevel()
{
    if(level > MIN_LEVEL)
        --level;
    
    return *this;
}