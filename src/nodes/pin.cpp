#include "pin.h"

minicrypto::PinInfo::PinInfo(const ax::NodeEditor::PinKind type, std::string text) : type(type)
{
  if (text.empty())
  {
    if (type == ax::NodeEditor::PinKind::Input)
      text = "-> in";
    else
      text = "out ->";
  }

  this->text = text;
}

ax::NodeEditor::PinId   minicrypto::PinInfo::get_id() const
{
  return id;
}

ax::NodeEditor::PinKind minicrypto::PinInfo::get_type() const
{
  return type;
}

std::string minicrypto::PinInfo::get_text() const
{
  return text;
}