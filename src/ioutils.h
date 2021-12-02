#pragma once

#include "types.h"

#include <string>
#include <vector>

namespace minicrypto
{
  std::vector<std::string> read_lines_from_file(
    const std::string& path
  );

  std::string read_all_from_file(
    const std::string& path
  );

  // Reads entire file, removes linechanges
  byte_string read_line_split_data_file(
    const std::string& path
  );

  std::string find_project_directory();
}
