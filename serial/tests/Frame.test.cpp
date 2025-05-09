/**
 * @file Frame.test.cpp
 * @author Adrian Szczepanski
 * @date 2025-05-06
 */

#include <libserial/Frame.hpp>

#include <CppUTest/TestHarness.h>

using namespace serial;

TEST_GROUP(FrameTest)
{
    std::vector<uint8_t> frameBuffer = { Frame::START_FLAG, 0x01, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG };
    std::vector<uint8_t> payload = { 0xAB, 0xCD };

	void setup()
	{

	}

	void teardown()
	{
		
	}
};

TEST(FrameTest, serialize)
{
    auto& expected = frameBuffer;
    Frame frame(0x01, payload);

    auto serializer = frame.serialize();
    auto serialized = serializer.getData();

    CHECK_EQUAL(expected.size(), serialized.size());
    MEMCMP_EQUAL(expected.data(), serialized.data(), expected.size());
}

TEST(FrameTest, find)
{
    auto [result, iterator] = Frame::find(frameBuffer);
    Frame frame(0x01, payload);
    etl::span<const uint8_t> bufferSpan(frameBuffer.data(), frameBuffer.size());

    CHECK(result.has_value());
    CHECK(frame == result.value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_noStart)
{
    std::vector<uint8_t> buffer = { 0x01, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK_FALSE(result.has_value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_noEnd)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, 0x01, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK_FALSE(result.has_value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_noType)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK_FALSE(result.has_value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_noPayloadStart)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, 0x01, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK_FALSE(result.has_value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_noPayloadEnd)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, 0x01, Frame::PAYLOAD_START, 0xAB, 0xCD, 0x15, 0xBF, Frame::END_FLAG };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK_FALSE(result.has_value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_noPayload)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, 0x01, Frame::PAYLOAD_START, Frame::PAYLOAD_END, 0xFF, 0xFF, Frame::END_FLAG };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());
    Frame frame(0x01, {});

    auto [result, iterator] = Frame::find(buffer);

    CHECK(result.has_value());
    CHECK(frame == result.value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_noCrc)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, 0x01, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, Frame::END_FLAG };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK_FALSE(result.has_value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_invalidCrc)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, 0x01, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x00, 0x00, Frame::END_FLAG };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK_FALSE(result.has_value());
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_withGarbage)
{
    std::vector<uint8_t> bufferWithGarbage = { Frame::START_FLAG, 0x01, Frame::END_FLAG,
        0x01, 0x02, 0x03,
        Frame::PAYLOAD_START,
        0xAB, 0xCD,
        Frame::START_FLAG, 0x01, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG,
        Frame::PAYLOAD_END,
        0x15, 0xBF};
    etl::span<const uint8_t> bufferSpan(bufferWithGarbage.data(), bufferWithGarbage.size());
    std::vector<uint8_t> payload = { 0xAB, 0xCD };

    auto [result, iterator] = Frame::find(bufferWithGarbage);

    CHECK(result.has_value());
    CHECK(result.has_value());
    CHECK(Frame(0x01, payload) == result.value()); 
    CHECK(iterator == std::prev(bufferSpan.end(), 3));
}

TEST(FrameTest, find_twoFrames)
{
    std::vector<uint8_t> bufferWithGarbage = { Frame::START_FLAG, 0x01, Frame::END_FLAG,
        0x01, 0x02, 0x03,
        Frame::PAYLOAD_START,
        0xAB, 0xCD,
        Frame::START_FLAG, 0x01, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG,
        Frame::PAYLOAD_END,
        0x15, 0xBF,
        Frame::START_FLAG, 0x02, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG,
        Frame::PAYLOAD_END,
        0x15, 0xBF};
    etl::span<const uint8_t> bufferSpan(bufferWithGarbage.data(), bufferWithGarbage.size());
    std::vector<uint8_t> payload = { 0xAB, 0xCD };

    auto [result, iterator] = Frame::find(bufferWithGarbage);

    CHECK(result.has_value());
    CHECK(Frame(0x01, payload) == result.value()); 

    auto [result2, iterator2] = Frame::find(etl::span<const uint8_t>{iterator, bufferSpan.end()});

    CHECK(result2.has_value());
    CHECK(Frame(0x02, payload) == result2.value());
    CHECK(iterator2 == std::prev(bufferSpan.end(), 3));
}

TEST(FrameTest, find_withTypeOfEndFlag)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, Frame::END_FLAG, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG };
    std::vector<uint8_t> payload = { 0xAB, 0xCD };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK(result.has_value());
    CHECK(result.has_value());
    CHECK(Frame(Frame::END_FLAG, payload) == result.value()); 
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_withTypeOfStartFlag)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, Frame::START_FLAG, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG };
    std::vector<uint8_t> payload = { 0xAB, 0xCD };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK(result.has_value());
    CHECK(result.has_value());
    CHECK(Frame(Frame::START_FLAG, payload) == result.value()); 
    CHECK(iterator == bufferSpan.end());
}

TEST(FrameTest, find_withTypeOfPayloadStartFlag)
{
    std::vector<uint8_t> buffer = { Frame::START_FLAG, Frame::PAYLOAD_START, Frame::PAYLOAD_START, 0xAB, 0xCD, Frame::PAYLOAD_END, 0x15, 0xBF, Frame::END_FLAG };
    std::vector<uint8_t> payload = { 0xAB, 0xCD };
    etl::span<const uint8_t> bufferSpan(buffer.data(), buffer.size());

    auto [result, iterator] = Frame::find(buffer);

    CHECK(result.has_value());
    CHECK(result.has_value());
    CHECK(Frame(Frame::PAYLOAD_START, payload) == result.value()); 
    CHECK(iterator == bufferSpan.end());
}