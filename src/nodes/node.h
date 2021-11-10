#pragma once

#include "../global.h"
#include "pin.h"
#include <vector>

namespace minicrypto
{
  class NodeInfo
  {
    ax::NodeEditor::NodeId id;
    std::vector<PinInfo> pins;

   public:
    NodeInfo(std::vector<PinInfo> pins={});

    void update();
  };
}
