#pragma once

#include "node.h"


namespace minicrypto
{
  class InputLineNode : public NodeInfo
  {
   public:
    InputLineNode();

    void update() override;
  };
}
