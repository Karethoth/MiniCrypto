#pragma once

#include "../global.h"

#include <vector>
#include <imgui.h>
#include "../../3rdparty/imgui-node-editor/imgui_node_editor.h"

namespace minicrypto
{
  class PinInfo
  {
    ax::NodeEditor::PinId id = imgui_resource_id_counter++;
    ax::NodeEditor::PinKind type;

   public:
    PinInfo(const ax::NodeEditor::PinKind type);
    ax::NodeEditor::PinId   get_id() const;
    ax::NodeEditor::PinKind get_type() const;
  };
}
