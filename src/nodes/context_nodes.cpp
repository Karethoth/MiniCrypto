#include "context_nodes.h"

void minicrypto::ContextNodes::link(LinkInfo link)
{
  links.push_back(link);
  link.register_link();
}

void minicrypto::ContextNodes::add(std::unique_ptr<NodeInfo> node)
{
  nodes.push_back(std::move(node));
}

void minicrypto::ContextNodes::remove(ax::NodeEditor::NodeId node_id)
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

bool minicrypto::ContextNodes::has_pin(ax::NodeEditor::PinId pin_id) const
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

bool minicrypto::ContextNodes::has_link(ax::NodeEditor::LinkId link_id) const
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

bool minicrypto::ContextNodes::accept_link(LinkInfo link)
{
  std::optional<NodeInfo> input_node;
  std::optional<NodeInfo> output_node;
  std::optional<PinInfo> input_pin;
  std::optional<PinInfo> output_pin;

  for (const auto& node : nodes)
  {
    const auto tmp_input_pin  = node->get_pin(link.get_input_id());
    const auto tmp_output_pin = node->get_pin(link.get_output_id());

    if (tmp_input_pin.has_value())
    {
      input_node = *node;
      input_pin = tmp_input_pin;
    }

    if (tmp_output_pin.has_value())
    {
      output_node = *node;
      output_pin = tmp_output_pin;
    }
  }

  if (!input_node.has_value() ||
      !output_node.has_value())
  {
    return false;
  }

  // Prevent links within the same node
  if (input_node->get_id() == output_node->get_id())
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
  if (input_pin->get_type() == ax::NodeEditor::PinKind::Output)
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
    if (existing_link.get_input_id() == input_pin->get_id())
    {
      return false;
    }
  }

  // TODO: Have both nodes accept the link

  this->link({input_pin->get_id(), output_pin->get_id()});
  return true;
}

bool minicrypto::ContextNodes::remove_link(ax::NodeEditor::LinkId link_id)
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
      link = links.erase(link);
      ax::NodeEditor::AcceptDeletedItem();
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
