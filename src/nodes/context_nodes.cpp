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
  // Accept all links for now
  ax::NodeEditor::AcceptNewItem();

  this->link(link);
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
