#include "utils.h"

using namespace minicrypto;

byte_string minicrypto::hex_to_byte_string(hex_string input)
{
  return {"\x01"};
}