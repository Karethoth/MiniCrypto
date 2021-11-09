#include "tests.h"

using namespace minicrypto;

TEST_CASE("Hex to byte conversion", "[HexToByte]")
{
  REQUIRE(hex_to_byte_string("01") == byte_string{"\x01"});
  REQUIRE(hex_to_byte_string("FF") == byte_string{"\xFF"});
}
