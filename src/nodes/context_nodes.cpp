#include "context_nodes.h"


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

void minicrypto::ContextNodes::update()
{
  for (auto& node : nodes)
  {
    node->update();
  }
}
