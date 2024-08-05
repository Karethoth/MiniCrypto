# Catch2
if(NOT Catch2_POPULATED)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.0.0-preview3
  )

  FetchContent_MakeAvailable(Catch2)
endif()

include(../cmake_helpers/config.cmake)
include(../cmake_helpers/openssl.cmake)

# Add ImGui, ImNodes, SDL2, and OpenGL
include_directories(
  ${CMAKE_SOURCE_DIR}/3rdparty/imgui
  ${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends
  ${CMAKE_SOURCE_DIR}/3rdparty/imnodes
  ${SDL2_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
)

set(IMGUI_SOURCES
  "${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui.cpp"
  "${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui_draw.cpp"
  "${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui_tables.cpp"
  "${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui_widgets.cpp"
  "${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_sdl2.cpp"
  "${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl2.cpp"
)

set(IMNODES_SOURCES
  "${CMAKE_SOURCE_DIR}/3rdparty/imnodes/imnodes.cpp"
)

# Tests
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE 1)
set(PROJECT_SOURCES
  "../src/frequency_analysis.cpp"
  "../src/ioutils.cpp"
  "../src/utils.cpp"
  "../src/types.cpp"
  "../src/nodes/context_nodes.cpp"
  "../src/nodes/node.cpp"
  "../src/nodes/link.cpp"
  "../src/nodes/pin.cpp"
  "../src/global.cpp"
)

add_executable(tests core_tests.cpp exercises_tests.cpp ${PROJECT_SOURCES} ${IMGUI_SOURCES} ${IMNODES_SOURCES})

set_property(TARGET tests PROPERTY CXX_STANDARD 17)

include_directories(
  ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(tests
  PRIVATE Catch2::Catch2WithMain
  ${OPENSSL_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${OPENGL_LIBRARIES}
)

target_compile_definitions(tests PRIVATE TESTING)

# DLLS for windows
if (WIN32)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDL2_LIB_DIR "x64")
    set(GLEW_LIB_DIR "x64")
    set(OPENSSL_AFFIX "x64")
  else()
    set(SDL2_LIB_DIR "x86")
    set(GLEW_LIB_DIR "x86")
    set(OPENSSL_AFFIX "x86")
  endif()

  add_custom_command(TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/3rdparty/SDL2/lib/${SDL2_LIB_DIR}/SDL2.dll"
    $<TARGET_FILE_DIR:tests>/SDL2.dll)

  add_custom_command(TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/3rdparty/glew/bin/Release/${GLEW_LIB_DIR}/glew32.dll"
    $<TARGET_FILE_DIR:tests>/glew32.dll)

  add_custom_command(TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/3rdparty/openssl/libcrypto-3-${OPENSSL_AFFIX}.dll"
    $<TARGET_FILE_DIR:tests>/libcrypto-3-${OPENSSL_AFFIX}.dll)
endif()

add_test(run_tests tests)

