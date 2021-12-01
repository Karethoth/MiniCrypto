#include "utils.h"
#include "frequency_analysis.h"

#include <unordered_map>

using namespace minicrypto;

ValueWithConfidence::ValueWithConfidence(
  byte_string value,
  float confidence
)
: value(value), confidence(confidence)
{
}

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

  throw std::out_of_range(
    "hexchar_to_nibble(): Input was not within range Aa-Zz|0-9!"
  );
}

byte_string minicrypto::hex_to_byte_string(const hex_string &input)
{
  byte_string result = "";

  // Verify string length is an even number, 2 or above
  if (input.size() % 2 != 0)
  {
    throw std::runtime_error(
      "hex_to_byte_string(): Can't convert uneven number of characters to a byte string!"
    );
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
    throw std::out_of_range("nibble_to_hexchar(): Input was not within range 0x0-0xF");
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

const uint8_t _base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

base64_string minicrypto::byte_to_base64_string(const byte_string &input)
{
  base64_string result = "";
  uint8_t work_bits = 0;
  uint8_t work_bits_count = 0;
  for (const auto &c : input)
  {
    // Add as many bits to the work_bits as can fit
    work_bits  |= c >> (2 + work_bits_count);

    // Using the six rightmost bits as the index, grab character from the table
    result += _base64_table[0b00111111 & work_bits];

    // Place leftover bits from c to the work_bits
    work_bits = c;

    // work_bits_count is always incremented by two, since we are consuming 6/8 bits when adding a new Base64 character
    work_bits_count += 2;

    // In case we have 6 characters ready in the work_bits, we can add the character rightaway
    if (work_bits_count >= 6)
    {
      result += _base64_table[0b00111111 & work_bits];
      work_bits_count = 0;
    }

    // Make room for the next bits
    work_bits <<= 6 - work_bits_count;
  }

  // Print remining bits and append '='s if necessary
  if (work_bits_count > 0)
  {
    result += _base64_table[0b00111111 & work_bits];
    if (work_bits_count == 2)
    {
      result += "==";
    }
    else if (work_bits_count == 4)
    {
      result += "=";
    }
  }

  return result;
}


byte_string minicrypto::xor_byte_string(
  byte_string input_a,
  const uint8_t key
)
{
  byte_string input_b{};
  input_b += key;

  return minicrypto::xor_byte_strings(input_a, input_b);
}

byte_string minicrypto::xor_byte_strings(
  byte_string input_a,
  const byte_string& input_b
)
{
  size_t index = 0;
  
  for (auto& c_a : input_a)
  {
    c_a ^= input_b[index];
    index = (index + 1) % input_b.length();
  }

  return input_a;
}


byte_string minicrypto::to_upper(byte_string input)
{
  for (auto& byte : input)
  {
    byte = std::toupper(byte);
  }
  return input;
}

size_t minicrypto::hamming_distance(uint8_t a, uint8_t b)
{
  uint8_t diff = 0;
  uint8_t xor  = a ^ b;

  while (xor)
  {
    diff += xor & 1;
    xor >>= 1;
  }
  return diff;
}

size_t minicrypto::hamming_distance(
  const byte_string& input_a,
  const byte_string& input_b
)
{
  const auto length_a = input_a.length();
  const auto length_b = input_b.length();
  const auto shared_length = std::min(length_a, length_b);
  const auto tail_length   = std::max(length_a, length_b) - shared_length;

  size_t distance = tail_length * 8;

  for (auto i = 0; i < shared_length; ++i)
  {
    distance += hamming_distance(input_a[i], input_b[i]);
  }

  return distance;

}


ValueWithConfidence minicrypto::decrypt_single_char_xor(const byte_string& input)
{
  const auto analyzer = LetterFrequencyAnalyzer::generate_english();
  const auto analyzer_dict = LetterFrequencyAnalyzer::generate_english_dict();
  std::unordered_map<uint32_t, float> scores{};

  byte_string most_likely_string = "";
  float most_likely_score = 0;
  
  for (uint32_t key = 0; key <= 255; ++key)
  {
    const auto decrypt_trial = xor_byte_string(input, key);
    scores[key] = analyzer.evaluate(decrypt_trial)
                + analyzer_dict.evaluate(decrypt_trial);

    if (scores[key] > most_likely_score)
    {
      most_likely_score = scores[key];
      most_likely_string = decrypt_trial;
    }
  }

  return { most_likely_string, most_likely_score };
}

