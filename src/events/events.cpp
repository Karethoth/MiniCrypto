#include "events.h"


minicrypto::DataChangedEvent::DataChangedEvent()
: source_node_type(NodeType::Default),
  source_pin{},
  local_pin{},
  data{}
{
}

