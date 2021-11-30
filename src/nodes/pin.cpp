#include "pin.h"

minicrypto::PinInfo::PinInfo(
  const minicrypto::PinKind type,
  std::string text
)
: id(Global::imgui_resource_id_counter++),
  type(type)
{
  if (text.empty())
  {
    if (type == minicrypto::PinKind::Input)
      text = "-> in";
    else
      text = "out ->";
  }

  this->text = text;
}

minicrypto::PinId minicrypto::PinInfo::get_id() const
{
  return id;
}

minicrypto::PinKind minicrypto::PinInfo::get_type() const
{
  return type;
}

std::string minicrypto::PinInfo::get_text() const
{
  return text;
}

