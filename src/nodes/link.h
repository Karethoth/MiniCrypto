#pragma once

#include "../global.h"

#include <imgui.h>
#include "../../3rdparty/imgui-node-editor/imgui_node_editor.h"

namespace minicrypto
{
  class LinkInfo
  {
    ax::NodeEditor::LinkId id;
    ax::NodeEditor::PinId  input_id;
    ax::NodeEditor::PinId  output_id;

   public:
    LinkInfo(ax::NodeEditor::PinId input_id, ax::NodeEditor::PinId output_id);

    ax::NodeEditor::LinkId get_id() const;
    void register_link() const;
  };
}
