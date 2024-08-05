#pragma once

#include "../events/events.h"
#include "node.h"
#include <GL/glew.h>

namespace minicrypto
{
  class RenderNode : public NodeInfo
  {
   protected:
     GLuint texture;
     GLuint depthbuffer;
     GLuint framebuffer;
     float rotation_angle;

   public:
    RenderNode();
    virtual ~RenderNode();

    void setup_framebuffer();
    void cleanup();

    void update() override;
    void render();
  };
}

