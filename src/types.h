#pragma once

#include <iostream>

namespace minicrypto
{
  using hex_string    = std::string;
  using byte_string   = std::string;
  using base64_string = std::string;

  template <typename T>
  struct ValueWithConfidence
  {
    T value;
    float confidence;

    ValueWithConfidence(T value, float confidence)
    : value(value), confidence(confidence)
    {
    }
  };
}

