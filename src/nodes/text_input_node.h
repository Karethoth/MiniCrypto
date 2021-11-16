#pragma once

#include "../events/events.h"
#include "node.h"

#include <functional>

namespace minicrypto
{
  class TextInputNode : public NodeInfo
  {
   protected:
    size_t      text_buffer_size = 1024;
    std::string text_buffer;

    // TODO: Use std::map with unique keys so that detaching an event handler can be done easily
    std::vector<DataChangedEventHandler> event_handlers;

   public:
    TextInputNode();

    void update() override;
    std::string get_string() const;

    // TODO: When using std::map to store the event_handlers, should return the key
    void add_event_handler(DataChangedEventHandler event_handler);

    // TODO: Add remove function which uses the key returned by add_event_handler
  };
}

