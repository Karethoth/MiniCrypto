#pragma once

#include <string>
#include <functional>

namespace minicrypto
{
  struct DataChangedEvent
  {
    // TODO: Add more detail. Event type, source node and pin, etc.
    // TODO: Make generic. DataEvent class and so on.
    std::string data;
  };

  using DataChangedEventHandler = std::function<void(const DataChangedEvent&)>;
}
