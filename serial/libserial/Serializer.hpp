#pragma once

/**
 * @file Serializer.hpp
 * @author Adrian Szczepanski
 * @date 2025-05-06
 */

#include <cstdint>
#include <string>
#include <vector>

#include <etl/span.h>

#include <libserial/Serializable.hpp>

namespace serial
{
    class Serializer
    {
    public:
        static constexpr uint8_t MAX_LEVEL = 255;
        static constexpr uint8_t MIN_LEVEL = 128;

        Serializer();

        Serializer& delim();
        Serializer& put(char);

        Serializer& ascii(uint8_t);
        Serializer& ascii(uint16_t);
        Serializer& ascii(uint32_t);
        Serializer& ascii(uint64_t);
        Serializer& ascii(bool);

        Serializer& number(uint8_t);
        Serializer& number(uint16_t);
        Serializer& number(uint32_t);
        Serializer& number(uint64_t);
        
        Serializer& string(std::string_view);
        Serializer& string(const char* str);

        Serializer& data(etl::span<const uint8_t>);
        Serializer& object(const Serializable&);
        Serializer& serializer(const Serializer&);

        Serializer& container(etl::span<const Serializable>);
        Serializer& container(etl::span<const std::string>);

        template <typename T>
        Serializer& containerAscii(etl::span<const T>);

        template <typename T>
        Serializer& containerNumber(etl::span<const T>);

        Serializer& incrementLevel();
        Serializer& decrementLevel();

        inline const auto& getData() { return buffer; }
        inline void clear() { buffer.clear(); }

    protected:

    private:
        uint8_t level = MIN_LEVEL;
        std::vector<uint8_t> buffer;
    };


    template <typename T>
    Serializer& Serializer::containerAscii(etl::span<const T> objects)
    {
        incrementLevel();
        for (auto& obj : objects)
        {
            ascii(obj);
            delim();
        }
        decrementLevel();

        return *this;
    }

    template <typename T>
    Serializer& Serializer::containerNumber(etl::span<const T> objects)
    {
        incrementLevel();
        for (auto& obj : objects)
        {
            number(obj);
            delim();
        }
        decrementLevel();

        return *this;
    }
}
