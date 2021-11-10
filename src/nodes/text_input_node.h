#pragma once

#include "node.h"

#include <array>

namespace minicrypto
{
  class TextInputNode : public NodeInfo
  {
   protected:
    std::array<char, 1024> text_buffer;

   public:
    TextInputNode();

    void update() override;
  };
}
