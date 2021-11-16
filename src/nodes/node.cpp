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
      return a.get_id() < b.get_id();
    }

    return type_a == minicrypto::PinKind::Input;
  });
}

void minicrypto::NodeInfo::update()
{
  ImNodes::BeginNode(id);
  ImGui::Text("Node");
  draw_pins();
  ImNodes::EndNode();
}

void minicrypto::NodeInfo::draw_pins()
{
  bool printing_in_pins = true;

  ImGui::BeginGroup();
  for (auto &pin : pins)
  {
    if (pin.get_type() == minicrypto::PinKind::Input)
    {
      ImNodes::BeginInputAttribute(pin.get_id());
      ImGui::TextUnformatted(pin.get_text().c_str());
      ImNodes::EndInputAttribute();
    }
    else
    {
      if (printing_in_pins)
      {
        printing_in_pins = false;
	    ImGui::EndGroup();
	    ImGui::SameLine();
        ImGui::BeginGroup();
      }
      ImNodes::BeginOutputAttribute(pin.get_id());
      ImGui::TextUnformatted(pin.get_text().c_str());
      ImNodes::EndOutputAttribute();
    }
  }
  ImGui::EndGroup();
}

minicrypto::NodeId minicrypto::NodeInfo::get_id() const
{
  return id;
}

minicrypto::NodeType minicrypto::NodeInfo::get_type() const
{
  return type;
}

std::optional<minicrypto::PinInfo> minicrypto::NodeInfo::get_pin(const minicrypto::PinId pin_id) const
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

bool minicrypto::NodeInfo::handle_input_changed_event(minicrypto::PinId pin_id, const minicrypto::DataChangedEvent &e)
{
  throw std::exception("Not implemented: handle_input_changed_event");
  return false;
}

void minicrypto::NodeInfo::add_output_changed_event_listener(minicrypto::DataChangedEventListener event_listener)
{
  event_listeners.push_back(event_listener);
}

