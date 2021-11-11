#include "text_input_node.h"

minicrypto::TextInputNode::TextInputNode()
: minicrypto::NodeInfo()
{
  type = NodeType::TextInput;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(ax::NodeEditor::PinKind::Output);
  text_buffer.resize(text_buffer_size);
}

void minicrypto::TextInputNode::update()
{
  ax::NodeEditor::BeginNode(get_id());
  ImGui::Text("Text Input");

  // Due to canvas coordinates vs screen cordinates, the text can show up in totally wrong place.
  // TODO: Fix.
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
  ax::NodeEditor::EndNode();
}

void minicrypto::TextInputNode::add_event_handler(DataChangedEventHandler event_handler)
{
  event_handlers.push_back(event_handler);
}
