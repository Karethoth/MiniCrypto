#pragma once

#include "../global.h"
#include "types.h"
#include "node.h"

#include <imgui.h>
#include "../../3rdparty/imnodes/imnodes.h"

namespace minicrypto
{
  class LinkInfo
  {
    LinkId id;
    PinId  input_pin_id;
    PinId  output_pin_id;

    NodeInfo* input_node;
    NodeInfo* output_node;

   public:
    LinkInfo(
      NodeInfo *input_node,
      NodeInfo *output_node,
      PinId input_pin_id,
      PinId output_pin_id
    );

    LinkId get_id() const;
    PinId  get_input_pin_id() const;
    PinId  get_output_pin_id() const;
    void   register_link() const;
    void   connect_data();

    // TODO
    //bool unregister_link();
  };
}

