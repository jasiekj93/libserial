/**
 * @file Serializer.test.cpp
 * @author Adrian Szczepanski
 * @date 2025-05-06
 */

#include <libserial/Serializer.hpp>

#include <CppUTest/TestHarness.h>

using namespace serial;

TEST_GROUP(SerializerTest)
{
    class TestSerializable : public Serializable
    {
    public:
        TestSerializable() = default;

        void serialize(Serializer& serializer) const override
        {
            serializer.string(name)
                .delim()
                .incrementLevel();
            
            for (const auto& number : numbers)
                serializer.ascii(number).delim();
            
            serializer.decrementLevel();
        }

        std::string name;
        std::vector<uint8_t> numbers;
    };

	void setup()
	{

	}

	void teardown()
	{
		
	}
};

TEST(SerializerTest, serialize)
{
    std::vector<uint8_t> expected = { 'T', 'e', 's', 't', Serializer::MIN_LEVEL, 
        '0', 'F', Serializer::MIN_LEVEL, 
        '1', Serializer::MIN_LEVEL, 
        '0', '0', '0', '1', 
        0xAB, 0xCD };

    Serializer serializer;

    serializer.string("Test")
        .delim()
        .ascii(static_cast<uint8_t>(0x0F))
        .delim()
        .ascii(true)
        .delim()
        .ascii(static_cast<uint16_t>(1))
        .number(static_cast<uint16_t>(0xABCD));

    auto data = serializer.getData();

    CHECK_EQUAL(expected.size(), data.size());
    MEMCMP_EQUAL(expected.data(), data.data(), expected.size());
}

TEST(SerializerTest, serialize_changeLevel)
{
    std::vector<uint8_t> expected = { 'T', 'e', 's', 't', Serializer::MIN_LEVEL, 
        '0', 'F', Serializer::MIN_LEVEL + 1, 
        '1', Serializer::MIN_LEVEL, 
        '0', '0', '0', '1', 
        0xAB, 0xCD };

    Serializer serializer;

    serializer.string("Test")
        .delim()
        .incrementLevel()
        .ascii(static_cast<uint8_t>(0x0F))
        .delim()
        .ascii(true)
        .decrementLevel()
        .delim()
        .ascii(static_cast<uint16_t>(1))
        .number(static_cast<uint16_t>(0xABCD));

    auto data = serializer.getData();

    CHECK_EQUAL(expected.size(), data.size());
    MEMCMP_EQUAL(expected.data(), data.data(), expected.size());
}

TEST(SerializerTest, serialize_object)
{
    std::vector<uint8_t> expected = { 
        '1', Serializer::MIN_LEVEL,
        'T', 'e', 's', 't', Serializer::MIN_LEVEL + 1, 
        'A', 'B', Serializer::MIN_LEVEL + 2, 
        'C', 'D', Serializer::MIN_LEVEL + 2,
        Serializer::MIN_LEVEL,
        'W', 'F'};

    Serializer serializer;

    TestSerializable testObject;
    testObject.name = "Test";
    testObject.numbers = { 0xAB, 0xCD };

    serializer.ascii(true)
        .delim()
        .object(testObject)
        .delim()
        .string("WF");

    auto data = serializer.getData();

    CHECK_EQUAL(expected.size(), data.size());
    MEMCMP_EQUAL(expected.data(), data.data(), expected.size());
}