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

TEST_CASE("Bytes to base64 conversion", "[byte_to_base64_string]")
{
  REQUIRE(
    byte_to_base64_string("Many hands make light work.") ==
    base64_string{"TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu"}
  );
  REQUIRE(byte_to_base64_string("light work.") == base64_string{"bGlnaHQgd29yay4="});
  REQUIRE(byte_to_base64_string("light work") == base64_string{"bGlnaHQgd29yaw=="});
  REQUIRE(byte_to_base64_string("light wor") == base64_string{"bGlnaHQgd29y"});
  REQUIRE(byte_to_base64_string("light wo") == base64_string{"bGlnaHQgd28="});
  REQUIRE(byte_to_base64_string("light w") == base64_string{"bGlnaHQgdw=="});
}

TEST_CASE("Hex to base64 conversion", "[hex_to_base64_string]")
{
  const auto set_1_challenge_1_bytes = hex_to_byte_string("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
  const auto set_1_challenge_1_base64 = byte_to_base64_string(set_1_challenge_1_bytes);

  REQUIRE(set_1_challenge_1_base64 == "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
}

