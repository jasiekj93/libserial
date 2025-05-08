// /**
//  * @file Response.test.cpp
//  * @author Adrian Szczepanski
//  * @date 2025-05-06
//  */

// #include <libplskl/serial/Response.hpp>

// #include <CppUTest/TestHarness.h>

// using namespace plskl;
// using namespace plskl::serial;

// TEST_GROUP(ResponseTest)
// {

// 	void setup()
// 	{

// 	}

// 	void teardown()
// 	{
		
// 	}
// };

// TEST(ResponseTest, withShortTag)
// {
//     std::vector<uint8_t> data = { '0', '1', '\0', 'T', 'e', 's', 't', '\t', 'E', 'd', 'i', 't', 'i', 'o', 'n', '\t', '0', '1', '\0', 0x6C, 0xBC };
//     Response response(data);

//     CHECK(response.isValid());
//     CHECK(response.getCount() == 1);
// }

// TEST(ResponseTest, withShortTagAndBool)
// {
//     std::vector<uint8_t> data = { '0', '1', '\0', 'T', 'e', 's', 't', '\t', 'E', 'd', 'i', 't', 'i', 'o', 'n', '\t', '0', '1', '\0', '1', '\0', 0x7D, 0x65 };
//     Response response(data);

//     CHECK(response.isValid());
//     CHECK(response.getCount() == 2);
// }
