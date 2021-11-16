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
    std::string data;

    DataChangedEvent();
  };

  using DataChangedEventListener = std::function<void(const DataChangedEvent&)>;
}

