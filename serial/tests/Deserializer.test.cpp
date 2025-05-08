/**
 * @file Deserializer.test.cpp
 * @author Adrian Szczepanski
 * @date 2025-05-06
 */

#include <libserial/Deserializer.hpp>

#include <CppUTest/TestHarness.h>

using namespace serial;

TEST_GROUP(DeserializerTest)
{
	void setup()
	{

	}

	void teardown()
	{
		
	}
};

TEST(DeserializerTest, getCount)
{
    std::vector<uint8_t> data = { 'T', 'e', 's', 't', '\t', '0', 'F', '\t', '1', '\t', '0', '0', '0', '1', '\t', 0xAB, 0xCD };
    Deserializer deserializer(data, '\t');

    CHECK_EQUAL(5, deserializer.count());

    auto arg0 = deserializer.getString(0);
    auto arg1 = deserializer.getAsciiUint8(1);
    auto arg2 = deserializer.getAsciiBool(2);
    auto arg3 = deserializer.getAsciiUint16(3);
    auto arg4 = deserializer.getUint16(4);

    CHECK(arg0.has_value());
    CHECK(arg1.has_value());
    CHECK(arg2.has_value());
    CHECK(arg3.has_value());
    CHECK(arg4.has_value());

    CHECK_EQUAL("Test", arg0.value());
    CHECK_EQUAL(0x0F, arg1.value());
    CHECK_EQUAL(true, arg2.value());
    CHECK_EQUAL(1, arg3.value());
    CHECK_EQUAL(0xABCD, arg4.value());
}


