#include "link.h"

minicrypto::LinkInfo::LinkInfo(
  ax::NodeEditor::PinId input_id,
  ax::NodeEditor::PinId output_id
)
: id(Global::imgui_resource_id_counter++),
  input_id(input_id),
  output_id(output_id)
{}

ax::NodeEditor::LinkId minicrypto::LinkInfo::get_id() const
{
  return id;
}

ax::NodeEditor::PinId minicrypto::LinkInfo::get_input_id() const
{
  return input_id;
}

ax::NodeEditor::PinId minicrypto::LinkInfo::get_output_id() const
{
  return output_id;
}

void minicrypto::LinkInfo::register_link() const
{
  ax::NodeEditor::Link(id, input_id, output_id);
}
