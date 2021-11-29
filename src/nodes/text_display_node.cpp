#include "text_display_node.h"

#include <cstring>

minicrypto::TextDisplayNode::TextDisplayNode()
: minicrypto::NodeInfo("Text Display")
{
  type = NodeType::TextInput;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(PinKind::Input);
}

void minicrypto::TextDisplayNode::update()
{
  ImNodes::BeginNode(get_id());
  ImGui::Text(name.c_str());

  if (ImGui::InputTextMultiline(
    "",
    text_buffer.data(),
    text_buffer.size(),
    ImVec2(400, 100),
    ImGuiInputTextFlags_ReadOnly
  ))
  {
    // Text changed, call the event handlers
    DataChangedEvent e{};
    e.source_node_type = NodeType::TextInput;

    // Only one output pin, the event listener in Link will fill in the local_pin for any connected pins
    e.source_pin = pins.begin()->get_id();

    // Might get expensive. Consider using reference to the text_buffer if the buffer sizes grow large.
    e.data = text_buffer;

    // Remove any excess data. Think of the string as if it's an usual C-string
    e.data.resize(std::strlen(e.data.c_str()));

    for (auto &event_listener : event_listeners)
    {
      event_listener(e);
    }
  }
  draw_pins();
  ImNodes::EndNode();
}

std::string minicrypto::TextDisplayNode::get_string() const
{
    // Might get expensive. Consider using reference to the text_buffer if the buffer sizes grow large.
    std::string str = text_buffer;

    // Remove any excess data. Think of the string as if it's an usual C-string
    str.resize(std::strlen(str.c_str()));
    return str;
}

bool minicrypto::TextDisplayNode::handle_input_changed_event(
  PinId pin_id,
  const DataChangedEvent& e
)
{
  text_buffer = e.data;
  return true;
}

