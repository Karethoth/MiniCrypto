# Catch2
if(NOT Catch2_POPULATED)
  Include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.0.0-preview3
  )

  FetchContent_MakeAvailable(Catch2)
endif()

include(../cmake_helpers/config.cmake)
include(../cmake_helpers/openssl.cmake)

# Tests
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE 1)
set(PROJECT_SOURCES
  "../src/frequency_analysis.cpp"
  "../src/ioutils.cpp"
  "../src/utils.cpp"
  "../src/types.cpp"
)
add_executable(tests core_tests.cpp exercises_tests.cpp ${PROJECT_SOURCES})
set_property(TARGET tests PROPERTY CXX_STANDARD 17)

include_directories(
  ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(tests
  PRIVATE Catch2::Catch2WithMain
  ${OPENSSL_LIBRARIES}
)

# DLLS for windows
if (WIN32)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OPENSSL_AFFIX "x64")
  else()
    set(OPENSSL_AFFIX "x86")
  endif()

  add_custom_command(TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/3rdparty/openssl/libcrypto-3-${OPENSSL_AFFIX}.dll"
    $<TARGET_FILE_DIR:tests>/libcrypto-3-${OPENSSL_AFFIX}.dll)
endif()

add_test(run_tests tests)

