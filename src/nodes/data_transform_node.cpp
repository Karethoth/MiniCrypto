#include "data_transform_node.h"
#include "../utils.h"

#include <algorithm>

minicrypto::DataTransformNode::DataTransformNode()
: minicrypto::NodeInfo()
{
  type = NodeType::DataTransform;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(minicrypto::PinKind::Input);
  input_pin_id = pins.back().get_id();
  pins.emplace_back(minicrypto::PinKind::Output);
  output_pin_id = pins.back().get_id();
  data_buffer.clear();
}

void minicrypto::DataTransformNode::update()
{
  ImNodes::BeginNode(get_id());
  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("Transform");
  ImNodes::EndNodeTitleBar();

  // TODO: Have a dropdown menu or something to select the transformation type

  draw_pins();
  ImNodes::EndNode();
}

bool minicrypto::DataTransformNode::handle_input_changed_event(PinId pin_id, const minicrypto::DataChangedEvent& e)
{
  // Single input so perform the operation and trigger the data changed event listeners

  // Default to bytes_to_base64_string for now
  data_buffer = byte_to_base64_string(e.data);

  minicrypto::DataChangedEvent new_event{};
  new_event.source_node_type = this->type;
  new_event.source_pin = this->output_pin_id;
  new_event.data = data_buffer.c_str();

  // Trigger listeners
  for (auto& event_listener : event_listeners)
  {
    event_listener(new_event);
  }

  return true;
}

