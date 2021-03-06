#pragma once

#include "../events/events.h"
#include "node.h"

#include <functional>

namespace minicrypto
{
  class TextInputNode : public NodeInfo
  {
   protected:
    size_t      text_buffer_size = 1024;
    std::string text_buffer;

   public:
    TextInputNode();

    void update() override;
    std::string get_string() const;
  };
}

