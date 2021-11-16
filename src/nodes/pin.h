#pragma once

#include "../global.h"
#include "types.h"
#include "../../3rdparty/imnodes/imnodes.h"

#include <string>

namespace minicrypto
{
  class PinInfo
  {
    PinId id;
    PinKind type;
    std::string text;

   public:
    PinInfo(const PinKind type, std::string text={});

    PinId       get_id() const;
    PinKind     get_type() const;
    std::string get_text() const;
  };
}

