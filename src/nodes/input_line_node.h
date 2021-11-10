#pragma once

#include "node.h"

#include <array>

namespace minicrypto
{
  class InputLineNode : public NodeInfo
  {
   protected:
    std::array<char, 1024> text_buffer;

   public:
    InputLineNode();

    void update() override;
  };
}
