#include "text_input_node.h"

minicrypto::TextInputNode::TextInputNode()
: minicrypto::NodeInfo()
{
  text_buffer.fill(0x00);
  type = NodeType::TextInput;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(ax::NodeEditor::PinKind::Output);
}

void minicrypto::TextInputNode::update()
{
  ax::NodeEditor::BeginNode(get_id());
  ImGui::Text("Text Input");

  // Due to canvas coordinates vs screen cordinates, the text can show up in totally wrong place.
  // TODO: Fix.
  ImGui::InputTextMultiline("", text_buffer.data(), text_buffer.size(), ImVec2(400, 100));
  draw_pins();
  ax::NodeEditor::EndNode();
}
