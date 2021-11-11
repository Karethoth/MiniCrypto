#pragma once

#include "node.h"

#include <memory>
#include <optional>

namespace minicrypto
{
  // Collection of nodes that can interact with each other.
  // Provides functions to help with handling the interactions.
  // Has ownership of all the nodes given to it.
  class ContextNodes
  {
    std::vector<std::unique_ptr<NodeInfo>> nodes{};

   public:
    void add(std::unique_ptr<NodeInfo> node);
    void remove(ax::NodeEditor::NodeId node_id);

    void update();
  };
}
