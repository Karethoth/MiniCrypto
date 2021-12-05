#pragma once

#include <iostream>

namespace minicrypto
{
  using hex_string    = std::string;
  using byte_string   = std::string;
  using base64_string = std::string;

  template <typename T_Value, typename T_Confidence=float>
  struct ValueWithConfidence
  {
    T_Value value;
    T_Confidence confidence;

    ValueWithConfidence(T_Value value, T_Confidence confidence)
    : value(value), confidence(confidence)
    {
    }
  };
}

