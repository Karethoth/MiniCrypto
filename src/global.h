#pragma once

#include "sdl2.h"

namespace minicrypto
{
  struct Global
  {
    static int imgui_resource_id_counter;
    static SDL_Window *sdl_window;
    static SDL_GLContext gl_context;
  };
}
