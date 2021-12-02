#pragma once

#include <string>
#include <vector>

namespace minicrypto
{
  std::vector<std::string> read_lines_from_file(
    const std::string& path
  );

  std::string find_project_directory();
}
