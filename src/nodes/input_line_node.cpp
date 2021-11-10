#include "input_line_node.h"

minicrypto::InputLineNode::InputLineNode()
: type(NodeType::InputLine)
{
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(ax::NodeEditor::PinKind::Output);
}


void minicrypto::InputLineNode::update()
{
  ax::NodeEditor::BeginNode(id);
  ImGui::Text("Node");
  draw_pins();
  ax::NodeEditor::EndNode();
}
