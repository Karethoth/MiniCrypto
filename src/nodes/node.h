#pragma once

#include "../global.h"
#include "../events/events.h"
#include "node_type.h"
#include "pin.h"

#include <map>
#include <vector>
#include <optional>

namespace minicrypto
{
  class NodeInfo
  {
    NodeId id;

   protected:
    NodeType type;
    std::string name;
    std::vector<PinInfo> pins;

    // TODO: Use std::map with unique keys so that detaching an event handler can be done easily
    std::map<NodeId, DataChangedEventListener> event_listeners;

    ImVec2 node_size;
    float max_label_width;
    bool can_add_pins;

    void sort_pins();
    void update_dimensions();

    float max_width_of_remining_pins(
      std::vector<PinInfo>::const_iterator pin_it,
      const std::vector<PinInfo>::const_iterator end
    ) const;

   public:
    NodeInfo(
      std::string name = "Node",
      std::vector<PinInfo> pins = {},
      ImVec2 node_size = { 200, 0 },
      bool can_add_pins = true
    );

    NodeType get_type() const;
    NodeId get_id() const;

    PinId add_pin(PinKind pin_kind, const std::string& pin_text);
    std::optional<PinInfo> get_pin(const PinId pin_id) const;

    virtual void update();
    virtual void draw_pins();

    virtual bool handle_input_changed_event(PinId pin_id, const DataChangedEvent& e);

    void add_output_changed_event_listener(NodeId listener_id, DataChangedEventListener event_listener);
    void remove_output_changed_event_listener(NodeId listener_id);
  };
}

