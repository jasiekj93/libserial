/**
 * @file Crc.test.cpp
 * @author Adrian Szczepanski
 * @date 2025-05-06
 */

#include <libserial/Crc.hpp>

#include <CppUTest/TestHarness.h>

using namespace serial;

TEST_GROUP(CrcTest)
{
    std::vector<uint8_t> data = { 0x01, 0x02, 0x03, 0x04 };
    uint16_t expectedCrc = 11169; 

	void setup()
	{

	}

	void teardown()
	{
		
	}
};

TEST(CrcTest, calculate)
{
    uint16_t crc = Crc::calculate(data);
    CHECK_EQUAL(expectedCrc, crc);
}

TEST(CrcTest, validate)
{
    CHECK(Crc::validate(data, expectedCrc));
    CHECK_FALSE(Crc::validate(data, expectedCrc + 1)); // Test with an invalid CRC
}

TEST(CrcTest, empty)
{
    std::vector<uint8_t> emptyData = {};
    CHECK_FALSE(Crc::validate(emptyData, expectedCrc)); // Test with an invalid CRC
}
