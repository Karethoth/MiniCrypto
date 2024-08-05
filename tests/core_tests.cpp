#include "tests.h"

#include <algorithm>
#include <unordered_map>
#include "../src/nodes/context_nodes.h"

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

TEST_CASE("AES ECB encrypt", "[encrypt_aes_ecb]")
{
  REQUIRE(
    minicrypto::decrypt_aes_ecb(
      minicrypto::encrypt_aes_ecb(
        "YELLOW SUBMARINE",
        "YELLOW SUBMARINE"
      ),
      "YELLOW SUBMARINE"
    )
    == "YELLOW SUBMARINE"
  );

  auto encrypted =
    minicrypto::encrypt_aes_ecb(
      minicrypto::pkcs7_pad("YELLOW", 16),
      "YELLOW SUBMARINE"
    );

  auto decrypted =
    minicrypto::decrypt_aes_ecb(
      encrypted,
      "YELLOW SUBMARINE"
    );

  REQUIRE(
    minicrypto::pkcs7_unpad(
      decrypted,
      16
    )
    == "YELLOW"
  );

  encrypted =
    minicrypto::encrypt_aes_ecb(
      minicrypto::pkcs7_pad(
        "In CBC mode, each ciphertext block is added to the next "
        "plaintext block before the next call to the cipher core.",
        16
      ),
      "HELLO WORLD,SUP?"
    );

  REQUIRE(
    minicrypto::pkcs7_unpad(
      minicrypto::decrypt_aes_ecb(
        encrypted,
        "HELLO WORLD,SUP?"
      ),
      16
    )
    == "In CBC mode, each ciphertext block is added to the next "
       "plaintext block before the next call to the cipher core."
  );
}

TEST_CASE("Random key generator", "[generate_random_key]")
{
  REQUIRE(minicrypto::generate_random_key(0).size() == 0);
  REQUIRE(minicrypto::generate_random_key(1).size() == 1);
  REQUIRE(minicrypto::generate_random_key(10).size() == 10);
  REQUIRE(minicrypto::generate_random_key(16).size() == 16);
}

TEST_CASE("Link creation and destruction", "[link_management]")
{
  ContextNodes context_nodes;

  // Add nodes with pins to context_nodes
  auto input_node = std::make_unique<NodeInfo>("Input Node");
  PinId input_pin_id = input_node->add_pin(PinKind::Input, "Input");
  context_nodes.add(std::move(input_node));

  auto output_node = std::make_unique<NodeInfo>("Output Node");
  PinId output_pin_id = output_node->add_pin(PinKind::Output, "Output");
  context_nodes.add(std::move(output_node));

  // Create a link
  REQUIRE(context_nodes.accept_link(input_pin_id, output_pin_id) == true);

  // Verify the link exists
  auto links = context_nodes.get_links();
  REQUIRE(std::any_of(links.begin(), links.end(), [&](const LinkInfo& link) {
    return link.get_input_pin_id() == input_pin_id && link.get_output_pin_id() == output_pin_id;
  }));

  // Get the link ID (assuming get_link_id is a method to get the link ID)
  LinkId link_id = links.front().get_id();

  // Remove the link
  REQUIRE(context_nodes.remove_link(link_id) == true);

  // Verify the link no longer exists
  links = context_nodes.get_links();
  REQUIRE(std::none_of(links.begin(), links.end(), [&](const LinkInfo& link) {
    return link.get_id() == link_id;
  }));
}

