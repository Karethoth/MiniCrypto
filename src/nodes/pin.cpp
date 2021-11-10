#include "pin.h"

minicrypto::PinInfo::PinInfo(const ax::NodeEditor::PinKind type) : type(type) {}

ax::NodeEditor::PinId   minicrypto::PinInfo::get_id() const
{
  return id;
}
ax::NodeEditor::PinKind minicrypto::PinInfo::get_type() const
{
  return type;
}