#pragma once

#include "../events/events.h"
#include "node.h"
#include "../types.h"

#include <vector>
#include <functional>

namespace minicrypto
{
  using DataTransformFunction = std::function<byte_string(const byte_string&)>;

  struct DataTransformOption
  {
    std::string name;
    DataTransformFunction func;
    bool is_selected;

    DataTransformOption(std::string name, DataTransformFunction func, bool is_selected=false);
  };

  class DataTransformNode : public NodeInfo
  {
   private:
    PinId input_pin_id;
    PinId output_pin_id;

   protected:
    std::string data_buffer;
    DataTransformOption* selected_option;
    std::vector<DataTransformOption> transform_options;

    std::string error_msg;

   public:
    DataTransformNode();

    void update() override;
    std::string get_string() const;

    bool handle_input_changed_event(PinId pin_id, const DataChangedEvent &e) override;
  };
}

