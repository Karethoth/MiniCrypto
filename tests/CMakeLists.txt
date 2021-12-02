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

# Tests
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE 1)
set(PROJECT_SOURCES
  "../src/frequency_analysis.cpp"
  "../src/ioutils.cpp"
  "../src/utils.cpp"
  "../src/types.cpp"
)
add_executable(tests tests.cpp ${PROJECT_SOURCES})
set_property(TARGET tests PROPERTY CXX_STANDARD 17)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_test(run_tests tests)

