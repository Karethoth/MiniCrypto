#include "link.h"

minicrypto::LinkInfo::LinkInfo(
  PinId input_id,
  PinId output_id
)
: id(Global::imgui_resource_id_counter++),
  input_id(input_id),
  output_id(output_id)
{}

minicrypto::LinkId minicrypto::LinkInfo::get_id() const
{
  return id;
}

minicrypto::PinId minicrypto::LinkInfo::get_input_id() const
{
  return input_id;
}

minicrypto::PinId minicrypto::LinkInfo::get_output_id() const
{
  return output_id;
}

void minicrypto::LinkInfo::register_link() const
{
  ImNodes::Link(id, input_id, output_id);
}

