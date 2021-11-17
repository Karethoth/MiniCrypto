#include "minicrypto.h"

#include "global.h"
#include "nodes/node.h"
#include "nodes/text_input_node.h"
#include "nodes/text_display_node.h"
#include "nodes/data_transform_node.h"
#include "nodes/link.h"
#include "nodes/context_nodes.h"

#include <vector>
#include <memory>

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>
#include <stdio.h>

using minicrypto::Global;

int init_sdl()
{
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Setup window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
  Global::sdl_window = SDL_CreateWindow("MiniCrypto", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  SDL_GLContext gl_context = SDL_GL_CreateContext(Global::sdl_window);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  return 0;
}

void init_imgui()
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(Global::sdl_window, Global::gl_context);
  ImGui_ImplOpenGL2_Init();
}

void cleanup()
{
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(Global::gl_context);
  SDL_DestroyWindow(Global::sdl_window);
  SDL_Quit();
}

int main(int, char**)
{
  if (init_sdl() < 0)
  {
    return -1;
  }

  init_imgui();

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  const auto imnodes_context = ImNodes::CreateContext();
  ImNodes::StyleColorsDark();

  auto generate_basic_pins = [] {
    std::vector<minicrypto::PinInfo> pins{};
    pins.emplace_back(minicrypto::PinKind::Input, "-> in #1");
    pins.emplace_back(minicrypto::PinKind::Input, "-> in #2");
    pins.emplace_back(minicrypto::PinKind::Output, "#1 out ->");
    pins.emplace_back(minicrypto::PinKind::Output, "#2 out ->");
    pins.emplace_back(minicrypto::PinKind::Input, "-> in #3");
    return pins;
  };

  minicrypto::ContextNodes context_nodes{};

  bool done = false;
  while (!done)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
      {
        done = true;
      }
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(Global::sdl_window);
    ImGui::NewFrame();

    auto& io = ImGui::GetIO();


    ImVec2 window_pos{ 0, 0 };
    ImVec2 window_size
    {
      io.DisplaySize.x,
      io.DisplaySize.y
    };

    ImGui::SetNextWindowPos(window_pos);
    ImGui::SetNextWindowSize(window_size);

    ImGui::Begin(
      "##MiniCrypto", nullptr,
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoSavedSettings
    );

    ImGui::Columns(3);
    ImGui::SetColumnOffset(1, 200);
    ImGui::SetColumnOffset(2, window_size.x - 200);

    ImGui::Text("Workspaces");
    ImGui::NextColumn();
    ImGui::Spacing();
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImNodes::SetCurrentContext(imnodes_context);
    ImNodes::BeginNodeEditor();
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

    context_nodes.update();

    ImNodes::PopAttributeFlag();
    ImNodes::EndNodeEditor();

    ImGui::NextColumn();
    ImGui::Text("Add Nodes");
    if (ImGui::Button("Transform Node", ImVec2{ 200, 30 }))
    {
      auto node = std::make_unique<minicrypto::DataTransformNode>();
      context_nodes.add(std::move(node));
    }

    if (ImGui::Button("Text Input Node", ImVec2{ 200, 30 }))
    {
      auto node = std::make_unique<minicrypto::TextInputNode>();
      context_nodes.add(std::move(node));
    }

    if (ImGui::Button("Text Display Node", ImVec2{ 200, 30 }))
    {
      auto node = std::make_unique<minicrypto::TextDisplayNode>();
      context_nodes.add(std::move(node));
    }

    ImGui::End();


    minicrypto::PinId input_pin_id, output_pin_id;
    if (ImNodes::IsLinkCreated(&input_pin_id, &output_pin_id))
    {
      if (input_pin_id && output_pin_id)
      {
        if (context_nodes.has_pin(input_pin_id) &&
          context_nodes.has_pin(output_pin_id))
        {
          if (!context_nodes.accept_link(input_pin_id, output_pin_id))
          {
              // Link wasn't accepted for some reason
          }
        }
      }
    }

    minicrypto::LinkId deleted_link_id;
    if (ImNodes::IsLinkDestroyed(&deleted_link_id))
    {
      if (context_nodes.has_link(deleted_link_id))
      {
        if (!context_nodes.remove_link(deleted_link_id))
        {
            // Link wasn't removed for some reason
        }
      }
    }

    // Rendering
    ImGui::Render();

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(Global::sdl_window);

    /*
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
      SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
    */
  }

  // Cleanup
  ImNodes::DestroyContext(imnodes_context);

  cleanup();
  return 0;
}

