set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)


if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  find_package(X11 REQUIRED)

  if (NOT X11_Xi_FOUND)
    message(FATAL_ERROR "X11 Xi library is required")
  endif ()
else()
  set(SDL2_DIR cmake_helpers)
endif ()

find_package(SDL2 REQUIRED)

include(cmake_helpers/imgui.cmake)
include(cmake_helpers/openssl.cmake)

if (WIN32)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GLEW_LIB_DIR "x64")
  else()
    set(GLEW_LIB_DIR "x86")
  endif()

  set(GLEW_ROOT "${CMAKE_SOURCE_DIR}/3rdparty/glew")
  set(GLEW_INCLUDE_DIR "${GLEW_ROOT}/include")
  set(GLEW_LIBRARY "${GLEW_ROOT}/lib/Release/${GLEW_LIB_DIR}/glew32.lib")
  include_directories(${GLEW_INCLUDE_DIR})

  set(CURL_ROOT "${CMAKE_SOURCE_DIR}/3rdparty/curl")
  set(CURL_INCLUDE_DIRS "${CURL_ROOT}/include")
  set(CURL_LIBRARIES "${CURL_ROOT}/lib/libcurl.dll.a")
  include_directories(${CURL_INCLUDE_DIRS})
endif ()

