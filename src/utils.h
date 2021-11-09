#pragma once

#include "types.h"

namespace minicrypto
{
  uint8_t hexchar_to_nibble(uint8_t c);
  uint8_t nibble_to_hexchar(uint8_t c);

  byte_string hex_to_byte_string(const hex_string &input);
  hex_string byte_to_hex_string(const byte_string &input);

  base64_string byte_to_base64_string(const byte_string &input);
}