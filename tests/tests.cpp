#include "tests.h"

using namespace minicrypto;
using namespace std::string_literals;

TEST_CASE("Hexchar to nibble conversion", "[hexchar_to_nibble]")
{
  REQUIRE(hexchar_to_nibble('0') == '\x00');
  REQUIRE(hexchar_to_nibble('1') == '\x01');
  REQUIRE(hexchar_to_nibble('9') == '\x09');
  REQUIRE(hexchar_to_nibble('A') == '\x0A');
  REQUIRE(hexchar_to_nibble('F') == '\x0F');
}

TEST_CASE("Nibble to hexchar conversion", "[nibble_to_hexchar]")
{
  REQUIRE(nibble_to_hexchar('\x00') == '0');
  REQUIRE(nibble_to_hexchar('\x01') == '1');
  REQUIRE(nibble_to_hexchar('\x09') == '9');
  REQUIRE(nibble_to_hexchar('\x0A') == 'A');
  REQUIRE(nibble_to_hexchar('\x0F') == 'F');
}

TEST_CASE("Hex to byte conversion", "[hex_to_byte_string]")
{
  REQUIRE(hex_to_byte_string("") == byte_string{""});

  REQUIRE(hex_to_byte_string("01") == byte_string{"\x01"});
  REQUIRE(hex_to_byte_string("FF") == byte_string{"\xFF"});

  REQUIRE(hex_to_byte_string("FFAAFF") == byte_string{"\xFF\xAA\xFF"});
  REQUIRE(hex_to_byte_string("DEADBEEF") == byte_string{"\xDE\xAD\xBE\xEF"});

  REQUIRE(hex_to_byte_string("410041") == byte_string{"\x41\x00\x41"s});
}


TEST_CASE("Byte to Hex conversion", "[byte_to_hex_string]")
{
  REQUIRE(byte_to_hex_string("") == hex_string{""});

  REQUIRE(byte_to_hex_string("\x01") == hex_string{"01"});
  REQUIRE(byte_to_hex_string("\xFF") == hex_string{"FF"});

  REQUIRE(byte_to_hex_string("\xFF\xAA\xFF") == hex_string{"FFAAFF"});
  REQUIRE(byte_to_hex_string("\xDE\xAD\xBE\xEF") == hex_string{"DEADBEEF"});

  REQUIRE(byte_to_hex_string("\x41\x00\x41"s) == byte_string{"410041"});
}