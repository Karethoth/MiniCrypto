#pragma once

#include "../global.h"
#include "../../3rdparty/imgui-node-editor/imgui_node_editor.h"

#include <string>

namespace minicrypto
{
  class PinInfo
  {
    ax::NodeEditor::PinId id;
    ax::NodeEditor::PinKind type;
    std::string text;

   public:
    PinInfo(const ax::NodeEditor::PinKind type, std::string text={});

    ax::NodeEditor::PinId   get_id() const;
    ax::NodeEditor::PinKind get_type() const;
    std::string             get_text() const;
  };
}
