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

TEST_CASE("Base64 to bytes conversion", "[base64_to_byte_string]")
{
  REQUIRE(
    byte_to_base64_string("Many hands make light work.") ==
    base64_string{"TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu"}
  );
  REQUIRE(base64_to_byte_string(byte_to_base64_string("Man")) == byte_string("Man"));
  REQUIRE(base64_to_byte_string(byte_to_base64_string("Ma")) == byte_string("Ma"));
  REQUIRE(base64_to_byte_string(byte_to_base64_string("M")) == byte_string("M"));
  REQUIRE(base64_to_byte_string(byte_to_base64_string("light wo")) == byte_string("light wo"));
  REQUIRE(base64_to_byte_string(byte_to_base64_string("light w")) == byte_string("light w"));
}

TEST_CASE("Hamming distance", "[hamming_distance]")
{
  REQUIRE(hamming_distance("", "") == 0);
  REQUIRE(hamming_distance("", hex_to_byte_string("ffffff")) == 3 * 8);
  REQUIRE(hamming_distance(hex_to_byte_string("ffffff"), "") == 3 * 8);
  REQUIRE(hamming_distance(hex_to_byte_string("000000"), hex_to_byte_string("000000")) == 0);
  REQUIRE(hamming_distance(hex_to_byte_string("aaaaaa"), hex_to_byte_string("aaaaaa")) == 0);
  REQUIRE(hamming_distance(hex_to_byte_string("ffffff"), hex_to_byte_string("ffffff")) == 0);
  REQUIRE(hamming_distance("this is a test", "wokka wokka!!!") == 37);
}

TEST_CASE("1-1: Convert hex to base64", "[hex_to_base64_string]")
{
  const auto set_1_challenge_1_bytes = hex_to_byte_string(
    "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
  const auto set_1_challenge_1_base64 = byte_to_base64_string(set_1_challenge_1_bytes);

  REQUIRE(set_1_challenge_1_base64 == "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
}

TEST_CASE("1-2: Fixed XOR", "[xor_byte_strings]")
{
  REQUIRE(
    xor_byte_strings(
      hex_to_byte_string("1c0111001f010100061a024b53535009181c"),
      hex_to_byte_string("686974207468652062756c6c277320657965")
    )
    == hex_to_byte_string("746865206b696420646f6e277420706c6179")
  );
}

TEST_CASE("1-3: Single-byte XOR cipher", "[decrypt_single_char_xor]")
{
  const auto result = decrypt_single_char_xor(
    hex_to_byte_string("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736")
  );

  REQUIRE(result.value == "Cooking MC's like a pound of bacon");
}

TEST_CASE("1-4: Detect single-character XOR", "[decrypt_single_char_xor_from_file]")
{
  // TODO: Add find parent directory with a file search and use that to find the absolute path
  const auto lines = read_lines_from_file(find_project_directory() + "/data/1_4.txt");
  ValueWithConfidence<byte_string> current_best{ "", 0 };
  for (const auto& line : lines)
  {
    const auto best_guess = decrypt_single_char_xor(hex_to_byte_string(line));
    if (best_guess.confidence > current_best.confidence)
    {
      current_best = best_guess;
    }
  }

  REQUIRE(current_best.value == "Now that the party is jumping\n");
}

TEST_CASE("1-5: Implement repeating-key XOR", "[decrypt_single_char_xor_from_file]")
{
  REQUIRE(
    byte_to_hex_string(
      xor_byte_strings(
        "Burning 'em, if you ain't quick and nimble\n" \
        "I go crazy when I hear a cymbal",
        "ICE"
      )
    )
    ==
    to_upper(
      "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272" \
      "a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f"
    )
  );
}


TEST_CASE("1-6: Break repeating-key XOR", "[decrypt_repeating_key_xor]")
{
  const auto data = minicrypto::base64_to_byte_string(
    minicrypto::read_line_split_data_file(
      minicrypto::find_project_directory() + "/data/1_6.txt"
    )
  );
  auto result = minicrypto::decrypt_repeating_key_xor(data);

  const auto answer_data = minicrypto::read_all_from_file(
    minicrypto::find_project_directory() + "/data/answers/1_6.txt"
  );
  REQUIRE(result.value == answer_data);
}

