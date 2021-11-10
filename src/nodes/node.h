#pragma once

#include "../global.h"
#include "node_type.h"
#include "pin.h"

#include <vector>

namespace minicrypto
{
  class NodeInfo
  {
    ax::NodeEditor::NodeId id;

   protected:
    NodeType type;
    std::vector<PinInfo> pins;

   public:
    NodeInfo(std::vector<PinInfo> pins={});

    NodeType get_type() const;
    void update();
    void draw_pins();
  };
}
