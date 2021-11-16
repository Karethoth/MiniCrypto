#pragma once

#include "../events/events.h"
#include "node.h"

namespace minicrypto
{
  class DataTransformNode : public NodeInfo
  {
   protected:
    std::string data_buffer;

   public:
    DataTransformNode();

    void update() override;
    std::string get_string() const;

    bool handle_input_changed_event(PinId pin_id, const DataChangedEvent &e) override;
  };
}

