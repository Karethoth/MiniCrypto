#pragma once

#include "../global.h"
#include "../events/events.h"
#include "node_type.h"
#include "pin.h"

#include <vector>
#include <optional>

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
    ax::NodeEditor::NodeId get_id() const;
    std::optional<PinInfo> get_pin(const ax::NodeEditor::PinId pin_id) const;

    virtual void update();
    virtual void draw_pins();

    virtual bool handle_input_changed_event(DataChangedEvent e);
  };
}
