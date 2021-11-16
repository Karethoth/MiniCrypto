#pragma once

#include "../global.h"
#include "types.h"

#include <imgui.h>
#include "../../3rdparty/imnodes/imnodes.h"

namespace minicrypto
{
  class LinkInfo
  {
    LinkId id;
    PinId  input_id;
    PinId  output_id;

   public:
    LinkInfo(PinId input_id, PinId output_id);

    LinkId get_id() const;
    PinId  get_input_id() const;
    PinId  get_output_id() const;
    void   register_link() const;
  };
}

