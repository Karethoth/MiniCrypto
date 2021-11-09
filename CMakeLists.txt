# CMakeList.txt : CMake project for MiniCrypto, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("minicrypto")

include(CMakeFiles/3rdparty.cmake)
include(CMakeFiles/config.cmake)

include_directories(
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
  ${IMGUI_INCLUDE_DIR}
)

set (PROJECT_SOURCES
  "src/minicrypto.cpp"
  "src/utils.cpp"
)

set (IMGUI_SOURCES
  "3rdparty/imgui/backends/imgui_impl_sdl.cpp"
  "3rdparty/imgui/backends/imgui_impl_opengl2.cpp"
)

set (IMGUI_NODE_EDITOR_SOURCES
  "3rdparty/imgui-node-editor/crude_json.cpp"
  "3rdparty/imgui-node-editor/imgui_node_editor_api.cpp"
  "3rdparty/imgui-node-editor/imgui_node_editor.cpp"
  "3rdparty/imgui-node-editor/imgui_canvas.cpp"
)

add_executable (minicrypto
  ${PROJECT_SOURCES}
  ${IMGUI_SOURCES}
  ${IMGUI_NODE_EDITOR_SOURCES}
)

set_property(TARGET minicrypto PROPERTY CXX_STANDARD 17)

target_link_libraries(minicrypto
  ${OPENGL_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${IMGUI_LIBRARIES}
)

include(CTest)
add_subdirectory(tests)
