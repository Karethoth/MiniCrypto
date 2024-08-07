#include "node.h"

#include <algorithm>
#include <stdexcept>

minicrypto::NodeInfo::NodeInfo(
  std::string name,
  std::vector<PinInfo> pins,
  ImVec2 node_size,
  bool can_add_pins
)
: type(NodeType::Default),
  name(name),
  id(Global::imgui_resource_id_counter++),
  pins(pins),
  node_size(node_size),
  can_add_pins(can_add_pins)
{
  sort_pins();
  update_dimensions();
  event_listeners = std::map<NodeId, DataChangedEventListener>{};
}

void minicrypto::NodeInfo::sort_pins()
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

void minicrypto::NodeInfo::update_dimensions()
{
#ifdef TESTING
  return;
#endif
  
  max_label_width = 0.f;
  for (auto pin = pins.begin(); pin != pins.end(); ++pin)
  {
    if (pin->get_type() == minicrypto::PinKind::Input)
    {
      continue;
    }

    max_label_width = max_width_of_remining_pins(
      pin,
      pins.cend()
    );
    break;
  }
}

float minicrypto::NodeInfo::max_width_of_remining_pins(
  std::vector<minicrypto::PinInfo>::const_iterator pin_it,
  const std::vector<minicrypto::PinInfo>::const_iterator end
) const
{
#ifdef TESTING
  return 0.f;
#endif

  float result = 0.f;
  while (pin_it != end)
  {
    const float label_width = ImGui::CalcTextSize(pin_it->get_text().c_str()).x;
    result = label_width > result ? label_width : result;
    ++pin_it;
  }
  return result;
}

void minicrypto::NodeInfo::draw_pins()
{
#ifdef TESTING
  return;
#endif

  bool printing_in_pins = true;

  ImGui::BeginGroup();
  for (auto& pin : pins)
  {
    if (pin.get_type() == minicrypto::PinKind::Input)
    {
      ImNodes::BeginInputAttribute(pin.get_id());
      const float label_width = ImGui::CalcTextSize(pin.get_text().c_str()).x;
      ImGui::PushItemWidth(node_size.x - label_width);
      ImGui::TextUnformatted(pin.get_text().c_str());
      ImGui::PopItemWidth();
      ImNodes::EndInputAttribute();
    }
    else
    {
      if (printing_in_pins)
      {
        printing_in_pins = false;
        ImGui::EndGroup();
        ImGui::SameLine(node_size.x - max_label_width);
        ImGui::BeginGroup();
      }

      ImNodes::BeginOutputAttribute(pin.get_id());
      const float label_width = ImGui::CalcTextSize(pin.get_text().c_str()).x;
      ImGui::PushItemWidth(node_size.x - max_label_width + label_width);
      ImGui::TextUnformatted(pin.get_text().c_str());
      ImGui::PopItemWidth();
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

std::optional<minicrypto::PinInfo> minicrypto::NodeInfo::get_pin(
  const minicrypto::PinId pin_id
) const
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

minicrypto::PinId minicrypto::NodeInfo::add_pin(PinKind pin_kind, const std::string& pin_text)
{
  if (!can_add_pins)
  {
    return false;
  }

  auto pin = minicrypto::PinInfo(pin_kind, pin_text);
  pins.push_back(pin);

  sort_pins();
  update_dimensions();

  return pin.get_id();
}

bool minicrypto::NodeInfo::handle_input_changed_event(
  minicrypto::PinId pin_id,
  const minicrypto::DataChangedEvent &e
)
{
  throw std::runtime_error("Not implemented: handle_input_changed_event");
  return false;
}

void minicrypto::NodeInfo::add_output_changed_event_listener(
  minicrypto::NodeId listener_id,
  minicrypto::DataChangedEventListener event_listener
)
{
  if (event_listeners.find(listener_id) != event_listeners.end())
  {
    event_listeners.erase(listener_id);
  }

  event_listeners[listener_id] = event_listener;
}

bool minicrypto::NodeInfo::has_event_listener(NodeId listener_id) const
{
  return event_listeners.find(listener_id) != event_listeners.end();
}


void minicrypto::NodeInfo::remove_output_changed_event_listener(NodeId listener_id)
{
  if (event_listeners.find(listener_id) != event_listeners.end())
  {
    event_listeners.erase(listener_id);
  }
}

