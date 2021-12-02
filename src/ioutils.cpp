#include "ioutils.h"

#include <fstream>
#include <stdexcept>
#include <filesystem>

std::vector<std::string>
minicrypto::read_lines_from_file(const std::string& path)
{
  std::vector<std::string> lines;
  std::ifstream stream{ path };
  std::string line;

  if (!stream.is_open())
  {
    throw std::runtime_error(
      "Failed to open '" + path + "' from '" +
      std::filesystem::current_path().u8string() + "'"
    );
  }

  while (std::getline(stream, line))
  {
    lines.push_back(line);
  }
  return lines;
}


std::string minicrypto::find_project_directory()
{
  auto cwd = std::filesystem::current_path();
  while (cwd.has_parent_path())
  {
    if (std::filesystem::is_directory(cwd.u8string() + "/data"))
    {
      break;
    }

    cwd = cwd.parent_path();
  }
  return cwd.u8string();
}

