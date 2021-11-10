#include "link.h"

minicrypto::LinkInfo::LinkInfo(
  ax::NodeEditor::PinId input_id,
  ax::NodeEditor::PinId output_id
)
: id(imgui_resource_id_counter++),
  input_id(input_id),
  output_id(output_id)
{}

ax::NodeEditor::LinkId minicrypto::LinkInfo::get_id() const
{
  return id;
}

void minicrypto::LinkInfo::register_link() const
{
  ax::NodeEditor::Link(id, input_id, output_id);
}