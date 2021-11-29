#include "text_input_node.h"

#include <cstring>

minicrypto::TextInputNode::TextInputNode()
: minicrypto::NodeInfo("Text Input")
{
  type = NodeType::TextInput;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(PinKind::Output);
  text_buffer.resize(text_buffer_size);
  node_width = 100;
}

void minicrypto::TextInputNode::update()
{
  ImNodes::BeginNode(get_id());
  ImGui::Text(name.c_str());

  ImGui::PushItemWidth(node_width);
  if (ImGui::InputTextMultiline(
    "",
    text_buffer.data(),
    text_buffer.size(),
    ImVec2(400, 100)
  ))
  {
    // Text changed, call the event handlers
    DataChangedEvent e{};
    e.source_node_type = NodeType::TextInput;

    // Only one output pin, the event listener in Link will fill in the local_pin for any connected pins
    e.source_pin = pins.begin()->get_id();

    // Might get expensive. Consider using reference to the text_buffer if the buffer sizes grow large.
    e.data = get_string();

    // Remove any excess data. Think of the string as if it's an usual C-string
    e.data.resize(std::strlen(e.data.c_str()));

    for (auto &event_listener : event_listeners)
    {
      event_listener(e);
    }
  }
  ImGui::PopItemWidth();
  draw_pins();
  ImNodes::EndNode();
}

std::string minicrypto::TextInputNode::get_string() const
{
    // Might get expensive. Consider using reference to the text_buffer if the buffer sizes grow large.
    std::string str = text_buffer;

    // Remove any excess data. Think of the string as if it's an usual C-string
    str.resize(std::strlen(str.c_str()));
    return str;
}

