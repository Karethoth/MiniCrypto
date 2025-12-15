#pragma once

#include "../events/events.h"
#include "node.h"
#ifdef _WIN32
#include <GL/glew.h>
#else
#include <epoxy/gl.h>
#endif

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

