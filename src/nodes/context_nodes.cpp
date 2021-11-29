#include "context_nodes.h"

void minicrypto::ContextNodes::link(LinkInfo link)
{
  link.connect_data();
  links.push_back(link);
}

void minicrypto::ContextNodes::add(std::unique_ptr<NodeInfo> node)
{
  nodes.push_back(std::move(node));
}

void minicrypto::ContextNodes::remove(NodeId node_id)
{
  auto node = nodes.begin();
  while (node != nodes.end())
  {
    if ((*node)->get_id() == node_id)
    {
      node = nodes.erase(node);
    }
    else {
      ++node;
    }
  }
}

bool minicrypto::ContextNodes::has_pin(PinId pin_id) const
{
  for (const auto& node : nodes)
  {
    const auto pin = (node)->get_pin(pin_id);
    if (pin.has_value())
    {
      return true;
    }
  }
  return false;
}

bool minicrypto::ContextNodes::has_link(LinkId link_id) const
{
  for (const auto& link : links)
  {
    if (link.get_id() == link_id)
    {
      return true;
    }
  }
  return false;
}

bool minicrypto::ContextNodes::accept_link(
  const minicrypto::PinId input_pin_id,
  const minicrypto::PinId output_pin_id)
{
  std::optional<NodeInfo*> input_node;
  std::optional<NodeInfo*> output_node;
  std::optional<PinInfo> input_pin;
  std::optional<PinInfo> output_pin;

  for (const auto& node : nodes)
  {
    const auto tmp_input_pin  = node->get_pin(input_pin_id);
    const auto tmp_output_pin = node->get_pin(output_pin_id);

    if (tmp_input_pin.has_value())
    {
      input_node = node.get();
      input_pin = tmp_input_pin;
    }

    if (tmp_output_pin.has_value())
    {
      output_node = node.get();
      output_pin = tmp_output_pin;
    }
  }

  if (!input_node.has_value() ||
      !output_node.has_value())
  {
    return false;
  }

  // Prevent links within the same node
  if (input_node.value()->get_id() == output_node.value()->get_id())
  {
    return false;
  }

  // Prevent links between pins of the same type
  if (input_pin->get_type() == output_pin->get_type())
  {
    return false;
  }

  // Make sure the pins match their type
  // - Depending on which way the link is created they may be flipped
  if (input_pin->get_type() == PinKind::Output)
  {
    auto tmp_pin  = input_pin;
    auto tmp_node = input_node;

    input_pin  = output_pin;
    input_node = output_node;

    output_pin  = tmp_pin;
    output_node = tmp_node;
  }

  // Make sure the input pin hasn't already been connected
  for (const auto& existing_link : links)
  {
    if (existing_link.get_input_pin_id() == input_pin->get_id())
    {
      return false;
    }
  }

  // TODO: Have both nodes accept the link

  this->link({
    input_node.value(),
    output_node.value(),
    input_pin->get_id(),
    output_pin->get_id()
  });
  return true;
}

bool minicrypto::ContextNodes::remove_link(LinkId link_id)
{
  if (!has_link(link_id))
  {
    return false;
  }

  // Accept all removes for now
  auto link = links.begin();
  while (link != links.end())
  {
    if (link->get_id() == link_id)
    {
      // TODO: Remove the event listener from output_pin
      link = links.erase(link);
      return true;
    }

    ++link;
  }
  return false;
}

void minicrypto::ContextNodes::update()
{
  for (auto& node : nodes)
  {
    node->update();
  }

  for (auto &link : links)
  {
    link.register_link();
  }
}

