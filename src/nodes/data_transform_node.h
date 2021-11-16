#pragma once

#include "../events/events.h"
#include "node.h"

namespace minicrypto
{
  class DataTransformNode : public NodeInfo
  {
   private:
    PinId input_pin_id;
    PinId output_pin_id;

   protected:
    std::string data_buffer;

   public:
    DataTransformNode();

    void update() override;
    std::string get_string() const;

    bool handle_input_changed_event(PinId pin_id, const DataChangedEvent &e) override;
  };
}

