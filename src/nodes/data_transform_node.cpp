#include "data_transform_node.h"

minicrypto::DataTransformNode::DataTransformNode()
: minicrypto::NodeInfo()
{
  type = NodeType::DataTransform;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(ax::NodeEditor::PinKind::Input);
  pins.emplace_back(ax::NodeEditor::PinKind::Output);
  data_buffer.clear();
}

void minicrypto::DataTransformNode::update()
{
  ax::NodeEditor::BeginNode(get_id());
  ImGui::Text("Transform");

  // TODO: Have a dropdown menu or something to select the transformation type

  draw_pins();
  ax::NodeEditor::EndNode();
}

void minicrypto::DataTransformNode::add_event_handler(DataChangedEventHandler event_handler)
{
  event_handlers.push_back(event_handler);
}
