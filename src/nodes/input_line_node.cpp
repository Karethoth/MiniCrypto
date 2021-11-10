#include "input_line_node.h"

minicrypto::InputLineNode::InputLineNode()
: minicrypto::NodeInfo()
{
  text_buffer.fill(0x00);
  type = NodeType::InputLine;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(ax::NodeEditor::PinKind::Output);
}

void minicrypto::InputLineNode::update()
{
  ax::NodeEditor::BeginNode(get_id());
  ImGui::Text("InputLineNode");
  ImGui::InputTextMultiline("input", text_buffer.data(), text_buffer.size(),ImVec2(0,0));
  draw_pins();
  ax::NodeEditor::EndNode();
}
