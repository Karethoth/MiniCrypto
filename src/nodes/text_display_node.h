#pragma once

#include "../events/events.h"
#include "node.h"

namespace minicrypto
{
  class TextDisplayNode : public NodeInfo
  {
   protected:
    std::string text_buffer;

   public:
    TextDisplayNode();

    void update() override;
    std::string get_string() const;

    bool handle_input_changed_event(PinId pin_id, const DataChangedEvent& e) override;
  };
}

