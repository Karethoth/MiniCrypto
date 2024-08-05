#include "data_transform_node.h"
#include "../utils.h"

#include <algorithm>

minicrypto::DataTransformOption::DataTransformOption(
  std::string name,
  minicrypto::DataTransformFunction func,
  bool is_selected
)
: name(name), func(func), is_selected(is_selected)
{
}

minicrypto::DataTransformNode::DataTransformNode()
  : minicrypto::NodeInfo("Transform", {}, { 150, 0 })
{
  type = NodeType::DataTransform;
  pins = std::vector<minicrypto::PinInfo>{};
  pins.emplace_back(minicrypto::PinKind::Input);
  input_pin_id = pins.back().get_id();
  pins.emplace_back(minicrypto::PinKind::Output);
  output_pin_id = pins.back().get_id();
  data_buffer.clear();

  transform_options.emplace_back("base64", minicrypto::byte_to_base64_string, true);
  transform_options.emplace_back("hex_to_bytes", minicrypto::hex_to_byte_string);
  transform_options.emplace_back("to_upper", minicrypto::to_upper);
 
  selected_option = &(*transform_options.begin());

  update_dimensions();
}

void minicrypto::DataTransformNode::update()
{
  ImNodes::BeginNode(get_id());
  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted(name.c_str());
  ImNodes::EndNodeTitleBar();

  const char* selected_option_name = "Select";
  if (selected_option)
  {
    selected_option_name = selected_option->name.c_str();
  }

  ImGui::PushItemWidth(120);
  if (ImGui::BeginCombo("##combo", selected_option_name))
  {
    for (auto& option : transform_options)
    {
      if (ImGui::Selectable(option.name.c_str(), &option.is_selected))
      {
        selected_option = &option;
        ImGui::SetItemDefaultFocus();
        // TODO: Trigger connected output nodes to send out new DataChangedEvents
      }
      else if (selected_option != &option && option.is_selected)
      {
        option.is_selected = false;
      }
    }
    ImGui::EndCombo();
  }
  ImGui::PopItemWidth();

  if (!error_msg.empty())
  {
    ImGui::Text(error_msg.c_str());
  }

  draw_pins();
  ImNodes::EndNode();
}

bool minicrypto::DataTransformNode::handle_input_changed_event(
  PinId pin_id,
  const minicrypto::DataChangedEvent& e
)
{
  // Single input so perform the operation and trigger the data changed event listeners

  // Default to bytes_to_base64_string for now
  if (!selected_option)
  {
    return true;
  }

  try
  {
    data_buffer = selected_option->func(e.data);
    error_msg = "";
  }
  catch (const std::exception& e)
  {
    error_msg = e.what();
  }

  minicrypto::DataChangedEvent new_event{};
  new_event.source_node_type = this->type;
  new_event.source_pin = this->output_pin_id;
  new_event.data = data_buffer.c_str();

  // Trigger listeners
  for (auto& event_listener : event_listeners)
  {
    event_listener.second(new_event);
  }

  return true;
}

