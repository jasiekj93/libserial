#include "Crc.hpp"

using namespace serial;

uint16_t Crc::calculate(etl::span<const uint8_t> span)
{
    uint16_t crc = 0xFFFF; // Initial CRC value

    for(auto byte : span)
    {
        crc ^= byte; // XOR byte into least sig. byte of crc
        for (int j = 0; j < 8; ++j) 
        {
            if (crc & 0x0001) 
            {
                crc >>= 1;
                crc ^= 0xA001; // Polynomial
            } 
            else 
                crc >>= 1;
        }
    }

    return crc;
}

bool Crc::validate(etl::span<const uint8_t> span, uint16_t crc)
{
    return (calculate(span) == crc);
}