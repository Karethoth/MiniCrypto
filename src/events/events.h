#pragma once

#include "../nodes/node_type.h"
#include "../nodes/pin.h"

#include <string>
#include <functional>

namespace minicrypto
{
  struct DataChangedEvent
  {
    NodeType source_node_type;
    PinId source_pin;
    PinId local_pin;
    std::string data;

    DataChangedEvent();
  };

  using DataChangedEventHandler = std::function<void(const DataChangedEvent&)>;
}

