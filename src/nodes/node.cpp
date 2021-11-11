#include "node.h"

#include <algorithm>


minicrypto::NodeInfo::NodeInfo(std::vector<PinInfo> pins)
: type(NodeType::Default),
  id(Global::imgui_resource_id_counter++),
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
  draw_pins();
  ax::NodeEditor::EndNode();
}

void minicrypto::NodeInfo::draw_pins()
{
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
}

ax::NodeEditor::NodeId minicrypto::NodeInfo::get_id() const
{
  return id;
}

minicrypto::NodeType minicrypto::NodeInfo::get_type() const
{
  return type;
}

std::optional<minicrypto::PinInfo> minicrypto::NodeInfo::get_pin(const ax::NodeEditor::PinId pin_id) const
{
  std::optional<minicrypto::PinInfo> result{};

  for (const auto& pin : pins)
  {
    if (pin.get_id() == pin_id)
    {
      result = pin;
      break;
    }
  }

  return result;
}


