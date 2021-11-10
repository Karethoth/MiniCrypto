#include "node.h"

minicrypto::NodeInfo::NodeInfo(std::vector<PinInfo> pins) : pins(pins)
{
}

void minicrypto::NodeInfo::update()
{
  ax::NodeEditor::BeginNode(id);
    ImGui::Text("Node");
    for (auto &pin : pins)
    {
      ax::NodeEditor::BeginPin(pin.get_id(), pin.get_type());
      if (pin.get_type() == ax::NodeEditor::PinKind::Input)
        ImGui::Text("-> in");
      else
        ImGui::Text("out ->");
      ax::NodeEditor::EndPin();
    }
  ax::NodeEditor::EndNode();
}