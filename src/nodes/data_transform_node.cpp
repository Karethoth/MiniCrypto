#include "data_transform_node.h"
#include "../utils.h"

minicrypto::DataTransformNode::DataTransformNode()
: minicrypto::NodeInfo()
{
  type = NodeType::DataTransform;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(minicrypto::PinKind::Input);
  pins.emplace_back(minicrypto::PinKind::Output);
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

void minicrypto::DataTransformNode::add_event_handler(DataChangedEventHandler event_handler)
{
  event_handlers.push_back(event_handler);
}

bool minicrypto::DataTransformNode::handle_input_changed_event(minicrypto::DataChangedEvent e)
{
  // Single input so perform the operation and trigger the data changed event listeners

  // Default to bytes_to_base64_string for now
  data_buffer = byte_to_base64_string(e.data);

  minicrypto::DataChangedEvent new_event{};
  new_event.source_node_type = this->type;
  new_event.data = data_buffer;

  // Trigger handlers
  for (auto& event_handler : event_handlers)
  {
    event_handler(new_event);
  }

  return true;
}

