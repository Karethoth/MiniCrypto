#include "text_input_node.h"

#include <cstring>

minicrypto::TextInputNode::TextInputNode()
: minicrypto::NodeInfo()
{
  type = NodeType::TextInput;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(PinKind::Output);
  text_buffer.resize(text_buffer_size);
}

void minicrypto::TextInputNode::update()
{
  ImNodes::BeginNode(get_id());
  ImGui::Text("Text Input");

  if (ImGui::InputTextMultiline("", text_buffer.data(), text_buffer.size(), ImVec2(400, 100)))
  {
    // Text changed, call the event handlers
    DataChangedEvent e{};

    // Might get expensive. Consider using reference to the text_buffer if the buffer sizes grow large.
    e.data = text_buffer;

    // Remove any excess data. Think of the string as if it's an usual C-string
    e.data.resize(std::strlen(e.data.c_str()));

    for (auto &event_handler : event_handlers)
    {
      event_handler(e);
    }
  }
  draw_pins();
  ImNodes::EndNode();
}

void minicrypto::TextInputNode::add_event_handler(DataChangedEventHandler event_handler)
{
  event_handlers.push_back(event_handler);
}

std::string minicrypto::TextInputNode::get_string() const
{
    // Might get expensive. Consider using reference to the text_buffer if the buffer sizes grow large.
    std::string str = text_buffer;

    // Remove any excess data. Think of the string as if it's an usual C-string
    str.resize(std::strlen(str.c_str()));
    return str;
}

