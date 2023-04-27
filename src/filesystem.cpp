#include "filesystem.h"
#include <iostream>
#include <sstream>

mfsys::filesystem::filesystem(const std::filesystem::path &path) {
  if (!std::filesystem::exists(path)) throw std::runtime_error("Binary path does not exist");

  if (path.string().find(".app") != std::string::npos) {
    binaryPath = path.parent_path().parent_path() / "Resources";
  } else {
    binaryPath = path.parent_path();
  }

#ifdef DEBUG
  std::cout << "Binary path: " << binaryPath << std::endl;
#endif
}

mfsys::filesystem::~filesystem() = default;

std::filesystem::path mfsys::filesystem::getBinaryPath() const { return binaryPath; }

std::string mfsys::filesystem::get(const std::string &path) const {
  std::string result = binaryPath.string();

  char separator = std::filesystem::path::preferred_separator;
  std::string token;
  std::stringstream ss(path);

  while (std::getline(ss, token, separator)) {
    result += separator;
    result += token;
  }

#ifdef DEBUG
  std::cout << "get::Result => " << result << std::endl;
#endif

  return result;
}

shader mfsys::filesystem::createShader(const std::string &vertexPath, const std::string &fragmentPath) const {
  std::string vertex_shader = get(vertexPath);
  std::string fragment_shader = get(fragmentPath);

  return {vertex_shader.c_str(), fragment_shader.c_str()};
}