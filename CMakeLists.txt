# CMakeList.txt : CMake project for MiniCrypto, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("minicrypto")

include(cmake_helpers/3rdparty.cmake)
include(cmake_helpers/config.cmake)

include_directories(
  ${OPENGL_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
  ${IMGUI_INCLUDE_DIR}
)

set (PROJECT_SOURCES
  "src/global.cpp"
  "src/minicrypto.cpp"
  "src/utils.cpp"
  "src/nodes/node.cpp"
  "src/nodes/text_input_node.cpp"
  "src/nodes/data_transform_node.cpp"
  "src/nodes/pin.cpp"
  "src/nodes/link.cpp"
  "src/nodes/context_nodes.cpp"
  "src/events/events.cpp"
)

set (IMGUI_SOURCES
  "3rdparty/imgui/backends/imgui_impl_sdl.cpp"
  "3rdparty/imgui/backends/imgui_impl_opengl2.cpp"
)

set (IMNODES_SOURCES
  "3rdparty/imnodes/imnodes.cpp"
)

add_executable (minicrypto
  ${PROJECT_SOURCES}
  ${IMGUI_SOURCES}
  ${IMNODES_SOURCES}
)

set_property(TARGET minicrypto PROPERTY CXX_STANDARD 17)

target_link_libraries(minicrypto
  ${OPENGL_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${IMGUI_LIBRARIES}
)

# SDL2.dll for Windows
if (WIN32)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDL2_LIB_DIR "x64")
  else()
    set(SDL2_LIB_DIR "x86")
  endif()

  add_custom_command(TARGET minicrypto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/3rdparty/SDL2/lib/${SDL2_LIB_DIR}/SDL2.dll"
    $<TARGET_FILE_DIR:minicrypto>/SDL2.dll)
endif()

include(CTest)
add_subdirectory(tests)
