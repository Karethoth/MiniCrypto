#include "MiniCrypto.h"

#include "global.h"
#include "nodes/node.h"
#include "nodes/input_line_node.h"
#include "nodes/link.h"

#include <vector>
#include <memory>

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>

namespace ed = ax::NodeEditor;
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
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
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

  ed::Config config;
  config.SettingsFile = "Simple.json";
  ed::EditorContext* editor_context = ed::CreateEditor(&config);

  auto generate_basic_pins = [] {
    std::vector<minicrypto::PinInfo> pins{};
    pins.emplace_back(ax::NodeEditor::PinKind::Input, "-> in #1");
    pins.emplace_back(ax::NodeEditor::PinKind::Input, "-> in #2");
    pins.emplace_back(ax::NodeEditor::PinKind::Output, "#1 out ->");
    pins.emplace_back(ax::NodeEditor::PinKind::Output, "#2 out ->");
    pins.emplace_back(ax::NodeEditor::PinKind::Input, "-> in #3");
    return pins;
  };

  std::vector<std::unique_ptr<minicrypto::NodeInfo>> nodes{};
  nodes.push_back(std::make_unique<minicrypto::NodeInfo>(generate_basic_pins()));
  nodes.push_back(std::make_unique<minicrypto::NodeInfo>(generate_basic_pins()));
  nodes.push_back(std::make_unique<minicrypto::NodeInfo>(generate_basic_pins()));
  nodes.push_back(std::make_unique<minicrypto::InputLineNode>());

  std::vector<minicrypto::LinkInfo> links{};

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
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
    ImGui::Separator();
    ed::SetCurrentEditor(editor_context);
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));
    int uniqueId = 1;

    for (auto &node : nodes)
    {
      node->update();
    }

    for (auto &link : links)
    {
      link.register_link();
    }

    if (ed::BeginCreate())
    {
      ed::PinId input_pin_id, output_pin_id;
      if (ed::QueryNewLink(&input_pin_id, &output_pin_id))
      {
        if (input_pin_id && output_pin_id) // both are valid, let's accept link
        {
          if (ed::AcceptNewItem())
          {
            links.emplace_back(input_pin_id, output_pin_id);
            links.back().register_link();
          }
        }
      }
    }
    ed::EndCreate();

    if (ed::BeginDelete())
    {
      ed::LinkId deleted_link_id;
      while (ed::QueryDeletedLink(&deleted_link_id))
      {
        if (ed::AcceptDeletedItem())
        {
          auto link = std::begin(links);
          while (link != links.end())
          {
            if (link->get_id() == deleted_link_id)
            {
              links.erase(link);
              break;
            }
            else
            {
              ++link;
            }
          }
        }
      }
    }
    ed::EndDelete();

    ed::End();
    ed::SetCurrentEditor(nullptr);

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(Global::sdl_window);
  }

  // Cleanup
  ed::DestroyEditor(editor_context);

  cleanup();
  return 0;
}