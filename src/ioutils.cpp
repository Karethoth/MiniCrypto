#include "ioutils.h"

#include <fstream>
#include <stdexcept>
#include <filesystem>

std::vector<std::string> minicrypto::read_lines_from_file(const std::string& path)
{
  std::vector<std::string> lines;
  std::ifstream stream{ path };
  std::string line;

  if (!stream.is_open())
  {
    throw std::runtime_error("Failed to open '" + path + "' from '" + std::filesystem::current_path().u8string() + "'");
  }

  while (std::getline(stream, line))
  {
    lines.push_back(line);
  }
  return lines;
}

