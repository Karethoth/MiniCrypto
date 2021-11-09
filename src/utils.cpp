#include "utils.h"

using namespace minicrypto;

uint8_t minicrypto::hexchar_to_nibble(uint8_t c)
{
  if ('a' <= c && c <= 'f' )
  {
    return 0xA + c - 'a';
  }
  else if ('A' <= c && c <= 'F' )
  {
    return 0xA + c - 'A';
  }
  else if ('0' <= c && c <= '9' )
  {
    return c - '0';
  }

  throw new std::out_of_range("hexchar_to_nibble(): Input was not within range Aa-Zz|0-9!");
}

byte_string minicrypto::hex_to_byte_string(const hex_string &input)
{
  byte_string result = "";

  // Verify string length is two
  if (input.size() % 2 != 0)
  {
    throw new std::runtime_error("hex_to_byte_string(): Can't convert uneven number of characters to a byte string!");
  }

  for (size_t i=0; i < input.size(); i += 2)
  {
    const char a = input[i];
    const char b = input[i+1];

    result += hexchar_to_nibble(a) << 4 | hexchar_to_nibble(b);
  }

  return result;
}


uint8_t minicrypto::nibble_to_hexchar(uint8_t c)
{
  if (c > 0xF)
  {
    throw new std::out_of_range("nibble_to_hexchar(): Input was not within range 0x0-0xF");
  }

  if (c < 0xA)
  {
    return '0' + c;
  }
  else
  {
    return 'A' + c - 0xA;
  }
}

hex_string minicrypto::byte_to_hex_string(const byte_string &input)
{
  hex_string result = "";

  for (const auto &b : input)
  {
    result += nibble_to_hexchar(b >> 4 & 0x0F);
    result += nibble_to_hexchar(b      & 0x0F);
  }

  return result;
}
