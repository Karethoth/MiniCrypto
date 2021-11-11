#pragma once

#include "node.h"
#include "link.h"

#include <memory>
#include <optional>

namespace minicrypto
{
  // Collection of nodes and links that can interact with each other.
  // Provides functions to help with handling the interactions.
  // Has ownership of all the nodes given to it.
  class ContextNodes
  {
    std::vector<std::unique_ptr<NodeInfo>> nodes{};
    std::vector<LinkInfo> links{};

    // Performs the linking operation. Also registers the link for the current frame.
    void link(LinkInfo link);

   public:
    void add(std::unique_ptr<NodeInfo> node);
    void remove(ax::NodeEditor::NodeId node_id);

    // Returns true if pin belongs to this context
    bool has_pin(ax::NodeEditor::PinId pin_id) const;

    // Returns true if link belongs to this context
    bool has_link(ax::NodeEditor::LinkId link_id) const;

    // Returns true and performs linking if the context can accept the link
    bool accept_link(LinkInfo link);

    // Returns true and removes the link if the context can remove the link
    bool remove_link(ax::NodeEditor::LinkId link_id);

    void update();
  };
}
