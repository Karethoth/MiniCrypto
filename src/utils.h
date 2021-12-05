#pragma once

#include "types.h"

#include <vector>

namespace minicrypto
{
  uint8_t hexchar_to_nibble(uint8_t c);
  uint8_t nibble_to_hexchar(uint8_t c);

  byte_string hex_to_byte_string(const hex_string &input);
  hex_string byte_to_hex_string(const byte_string &input);

  base64_string byte_to_base64_string(const byte_string &input);
  byte_string   base64_to_byte_string(const base64_string &input);

  byte_string xor_byte_string(byte_string input_a, const uint8_t key);
  byte_string xor_byte_strings(byte_string input_a, const byte_string& input_b);
  byte_string to_upper(byte_string input);

  size_t hamming_distance(uint8_t a, uint8_t b);
  size_t hamming_distance(const byte_string& input_a, const byte_string& input_b);

  ValueWithConfidence<byte_string>
  decrypt_single_char_xor(const byte_string& input);

  std::vector<byte_string>
    get_blocks_of_size(
      const byte_string& input,
      const size_t blocksize,
      const size_t max_blocks = 0
    );

  ValueWithConfidence<size_t>
  guess_repeating_key_xor_length(
    const byte_string& input,
    const size_t samples = 4,
    const uint8_t min = 1,
    const uint8_t max = 40
  );

  ValueWithConfidence<byte_string>
  decrypt_repeating_key_xor(const byte_string& input);

  byte_string
  decrypt_aes_ecb(const byte_string& input, const byte_string& key);

  byte_string
  encrypt_aes_ecb(const byte_string& input, const byte_string& key);

  ValueWithConfidence<byte_string, size_t>
  find_most_repeated_block(
    const std::vector<byte_string>& blocks
  );

  byte_string pkcs7_pad(const byte_string& input, const size_t blocksize);
}

