#include "link.h"
#include "../events/events.h"
#include <utility>

minicrypto::LinkInfo::LinkInfo(
  NodeInfo* input_node,
  NodeInfo* output_node,
  PinId input_pin_id,
  PinId output_pin_id
)
: id(Global::imgui_resource_id_counter++),
  input_node(input_node),
  output_node(output_node),
  input_pin_id(input_pin_id),
  output_pin_id(output_pin_id)
{
  output_node->add_output_changed_event_listener([
    input_node,
    output_node,
    input_pin_id,
    output_pin_id
  ] (const minicrypto::DataChangedEvent& e)
  {
    if (e.source_pin != output_pin_id)
    {
      return;
    }

    input_node->handle_input_changed_event(output_pin_id, e);
  });
}

void minicrypto::LinkInfo::connect_data()
{
}

minicrypto::LinkId minicrypto::LinkInfo::get_id() const
{
  return id;
}

minicrypto::PinId minicrypto::LinkInfo::get_input_pin_id() const
{
  return input_pin_id;
}

minicrypto::PinId minicrypto::LinkInfo::get_output_pin_id() const
{
  return output_pin_id;
}

void minicrypto::LinkInfo::register_link() const
{
  ImNodes::Link(id, input_pin_id, output_pin_id);
}

