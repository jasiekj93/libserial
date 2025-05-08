#pragma once

/**
 * @file Crc.hpp
 * @author Adrian Szczepanski
 * @date 2025-05-06
 */

#include <cstdint>

#include <etl/span.h>

namespace serial
{
    class Crc
    {
    public:
        static uint16_t calculate(etl::span<const uint8_t>);
        static bool validate(etl::span<const uint8_t>, uint16_t crc);

    private:
        Crc() = delete;
    };
}