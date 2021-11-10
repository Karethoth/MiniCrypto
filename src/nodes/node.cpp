#include "node.h"

minicrypto::NodeInfo::NodeInfo(std::vector<PinInfo> pins) : pins(pins)
{
}

void minicrypto::NodeInfo::update()
{
  ax::NodeEditor::BeginNode(id);
    ImGui::Text("Node");
    // TODO: Have every other pin switch between an input and an output.
    // This would allow using ImGui::Sameline between them
    for (auto &pin : pins)
    {
      ax::NodeEditor::BeginPin(pin.get_id(), pin.get_type());
      ImGui::Text(pin.get_text().c_str());
      ax::NodeEditor::EndPin();
    }
  ax::NodeEditor::EndNode();
}