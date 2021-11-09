#include "tests.h"

using namespace minicrypto;

TEST_CASE("Hex to byte conversion", "[hex_to_byte_string]")
{
  REQUIRE(hex_to_byte_string("") == byte_string{""});

  REQUIRE(hex_to_byte_string("01") == byte_string{"\x01"});
  REQUIRE(hex_to_byte_string("FF") == byte_string{"\xFF"});

  REQUIRE(hex_to_byte_string("FFAAFF") == byte_string{"\xFF\xAA\xFF"});
  REQUIRE(hex_to_byte_string("DEADBEEF") == byte_string{"\xDE\xAD\xBE\xEF"});

  REQUIRE(hex_to_byte_string("010001") == byte_string{"\x01\x00\0x01"});
}
