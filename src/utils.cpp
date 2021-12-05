#include "utils.h"
#include "ioutils.h"

#include "frequency_analysis.h"

#include <unordered_map>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

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

const char _base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

byte_string minicrypto::base64_to_byte_string(const base64_string &input)
{
  byte_string result = "";
  uint16_t work_bits = 0;
  uint8_t work_bits_count = 0;
  for (const auto &c : input)
  {
    if (c == '=')
    {
      break;
    }

    uint8_t new_bits = static_cast<uint8_t>(
      strchr(_base64_table, c) - _base64_table
    );
    work_bits = (work_bits << 6) | (0b111111 & new_bits);
    work_bits_count += 6;

    if (work_bits_count >= 8)
    {
      const uint8_t extra_bits_count = work_bits_count - 8;
      const uint8_t data_bits = (work_bits >> extra_bits_count) & 0xFF;
      work_bits_count -= 8;

      result += data_bits;
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
  uint8_t xorred  = a ^ b;

  while (xorred)
  {
    diff += xorred & 1;
    xorred >>= 1;
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


ValueWithConfidence<minicrypto::byte_string>
minicrypto::decrypt_single_char_xor(const byte_string& input)
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
                + analyzer_dict.evaluate(decrypt_trial) / 2;

    if (scores[key] > most_likely_score)
    {
      most_likely_score = scores[key];
      most_likely_string = decrypt_trial;
    }
  }

  return { most_likely_string, most_likely_score };
}

std::vector<minicrypto::byte_string>
minicrypto::get_blocks_of_size(
  const minicrypto::byte_string& input,
  const size_t blocksize,
  const size_t max_blocks
)
{
  std::vector<minicrypto::byte_string> blocks;

  // Calculate how many blocks we can get
  const size_t available_blocks_count = (input.size() / blocksize);
  const size_t used_block_count =
    max_blocks == 0 ? available_blocks_count
                    : std::min(available_blocks_count, (size_t)max_blocks);

  // Gather the blocks
  for (auto i = 0; i < used_block_count; ++i)
  {
    const auto offset = i * blocksize;
    blocks.push_back(input.substr(offset, blocksize));
  }

  return blocks;
}

ValueWithConfidence<size_t>
minicrypto::guess_repeating_key_xor_length(
  const byte_string& input,
  const size_t samples,
  const uint8_t min,
  const uint8_t max
)
{
  ValueWithConfidence<size_t> current_best{ 0, 0 };
  for (auto key_length = min; key_length <= max; ++key_length)
  {
    const auto blocks = minicrypto::get_blocks_of_size(input, key_length, samples);

    // Calculate average hamming distance between the blocks
    size_t comparisons = 0;
    size_t hamming_distance_sum = 0;
    for (auto block_a = blocks.cbegin(); block_a != blocks.cend(); ++block_a)
    {
      for (auto block_b = block_a + 1; block_b != blocks.cend(); ++block_b)
      {
        ++comparisons;
        hamming_distance_sum += hamming_distance(*block_a, *block_b);
      }
    }

    // Normalize. Max average is 8
    const auto average = ((float)hamming_distance_sum / comparisons)
                       / key_length;

    const auto result = 1.f - average / 8.f;

    if (result > current_best.confidence)
    {
      current_best.confidence = result;
      current_best.value = key_length;
    }
  }

  return current_best;
}

std::unordered_map<size_t, minicrypto::byte_string>
split_data_into_columns(const minicrypto::byte_string &input, size_t n)
{
  std::unordered_map<size_t, minicrypto::byte_string> columns;

  size_t counter = 0;
  for (const auto& c : input)
  {
    const auto column = counter++ % n;
    columns[column].push_back(c);
  }

  return columns;
}

ValueWithConfidence<minicrypto::byte_string>
minicrypto::decrypt_repeating_key_xor(const byte_string& input)
{
  const auto best_length_guess = guess_repeating_key_xor_length(input);
  const auto data_columns = split_data_into_columns(input, best_length_guess.value);
  
  minicrypto::byte_string key = "";
  for (const auto& column : data_columns)
  {
    const auto decrypt_trial = decrypt_single_char_xor(column.second);
    const auto key_byte = decrypt_trial.value[0] ^ column.second[0];
    key += key_byte;
  }

  return { xor_byte_strings(input, key), 0};
}


minicrypto::byte_string
minicrypto::decrypt_aes_ecb(
  const minicrypto::byte_string& input,
  const minicrypto::byte_string& key
)
{
  minicrypto::byte_string output;
  output.resize(input.size() + 16);

  EVP_CIPHER_CTX* ctx;
  int len = 0, partial_len = 0;

  if (!(ctx = EVP_CIPHER_CTX_new()))
    throw "EVP_CIPHER_CTX_new failed";

  EVP_CIPHER_CTX_set_padding(ctx, 0);

  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, (const uint8_t*)key.data(), nullptr))
    throw "EVP_aes_128_ecb failed";

  if (1 != EVP_DecryptUpdate(ctx, (uint8_t*)output.data(), &len, (const uint8_t*)input.data(), static_cast<int>(input.size())))
    throw "EVP_DecryptUpdate failed";

  partial_len = len;
  if (partial_len > 0)
  {
    if (1 != EVP_DecryptFinal_ex(ctx, (uint8_t*)output.data() + len, &len)) 
      throw "EVP_DecryptFinal failed";
  }
  const auto byte_count = partial_len + len;

  EVP_CIPHER_CTX_free(ctx);

  return {
    output.begin(),
    output.begin() + byte_count
  };
}


minicrypto::byte_string
minicrypto::encrypt_aes_ecb(
  const minicrypto::byte_string& input,
  const minicrypto::byte_string& key
)
{
  minicrypto::byte_string output;
  output.resize(input.size() + 16);

  EVP_CIPHER_CTX* ctx;
  int len = 0, partial_len = 0;

  if (!(ctx = EVP_CIPHER_CTX_new()))
    throw "EVP_CIPHER_CTX_new failed";

  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, (const uint8_t*)key.data(), nullptr))
    throw "EVP_aes_128_ecb failed";

  if (1 != EVP_EncryptUpdate(ctx, (uint8_t*)output.data(), &len, (const uint8_t*)input.data(), static_cast<int>(input.size())))
    throw "EVP_DecryptUpdate failed";

  partial_len = len;

  if (1 != EVP_EncryptFinal_ex(ctx, (uint8_t*)output.data() + len, &len))
    throw "EVP_DecryptFinal failed";

  const auto byte_count = partial_len + len;

  EVP_CIPHER_CTX_free(ctx);

  return {
    output.begin(),
    output.begin() + byte_count
  };
}


minicrypto::ValueWithConfidence<minicrypto::byte_string, size_t>
minicrypto::find_most_repeated_block(
  const std::vector<minicrypto::byte_string>& blocks
)
{
  std::unordered_map<minicrypto::byte_string, size_t> encounters;
  for (const auto& block : blocks)
  {
    encounters[block]++;
  }

  ValueWithConfidence<minicrypto::byte_string, size_t> best_guess{ "", 0 };
  for (const auto& enc : encounters)
  {
    const auto duplicate_count = enc.second - 1;
    if (duplicate_count > best_guess.confidence)
    {
      best_guess.value = enc.first;
      best_guess.confidence = duplicate_count;
    }
  }

  return best_guess;
}

minicrypto::byte_string
minicrypto::pkcs7_pad(const minicrypto::byte_string& input, const size_t blocksize)
{
  if (input.size() > blocksize)
  {
    throw std::runtime_error("pkcs7_pad: input length > blocksize!");
  }

  if (blocksize - input.size() > 0xFF)
  {
    throw std::runtime_error("pkcs7_pad: (blocksize - input length) > 0xFF!");
  }

  const uint8_t filler_byte = static_cast<uint8_t>(blocksize - input.size());
  const auto padding = minicrypto::byte_string(filler_byte, filler_byte);
  return input + padding;
}


minicrypto::byte_string
minicrypto::decrypt_cbc(
  const minicrypto::byte_string& input,
  const minicrypto::byte_string& key,
  const minicrypto::byte_string& iv
)
{
  minicrypto::byte_string output{};

  auto blocks = minicrypto::get_blocks_of_size(input, 16);
  if (blocks.size() * 16 < input.size())
  {
    blocks.push_back(input.substr(blocks.size() * 16));
  }

  auto last_block = iv;

  for (const auto& block : blocks)
  {
    const auto decrypted = minicrypto::decrypt_aes_ecb(block, key);
    const auto plaintext = minicrypto::xor_byte_strings(decrypted, last_block);

    output += plaintext;

    last_block = block;
  }

  return output;
}

