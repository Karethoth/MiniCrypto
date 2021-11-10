#include "node.h"

#include <algorithm>


minicrypto::NodeInfo::NodeInfo(std::vector<PinInfo> pins)
: id(Global::imgui_resource_id_counter++),
  pins(pins)
{
  // Order pins by type and id for nice rendering
  std::sort(
    this->pins.begin(),
    this->pins.end(),
    [](auto a, auto b)
  {
    const auto type_a = a.get_type();
    const auto type_b = b.get_type();
    if (type_a == type_b)
    {
      return a.get_id().Get() < b.get_id().Get();
    }

    return type_a == ax::NodeEditor::PinKind::Input;
  });
}

void minicrypto::NodeInfo::update()
{
  ax::NodeEditor::BeginNode(id);
  ImGui::Text("Node");

  bool printing_in_pins = true;

  ImGui::BeginGroup();
  for (auto &pin : pins)
  {
    if (printing_in_pins && pin.get_type() == ax::NodeEditor::PinKind::Output)
    {
      printing_in_pins = false;
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
    }

    ax::NodeEditor::BeginPin(pin.get_id(), pin.get_type());
    ImGui::Text(pin.get_text().c_str());
    ax::NodeEditor::EndPin();
  }
  ImGui::EndGroup();
  ax::NodeEditor::EndNode();
}