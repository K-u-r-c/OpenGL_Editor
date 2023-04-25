#include "filesystem.h"
#include <iostream>
#include <sstream>

MyFilesystem::File::File(const std::filesystem::path& path) {
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

MyFilesystem::File::~File() {}

std::filesystem::path MyFilesystem::File::getBinaryPath() const { return binaryPath; }

std::string MyFilesystem::File::getByRelativePath(const std::string& path) const {
  std::string result = binaryPath.string();

  char separator = std::filesystem::path::preferred_separator;
  std::string token;
  std::stringstream ss(path);

  while (std::getline(ss, token, separator)) {
    result += separator;
    result += token;
  }

#ifdef DEBUG
  std::cout << "getByRelativePath::Result => " << result << std::endl;
#endif

  return result;
}

Shader MyFilesystem::File::createShader(const std::string& vertexPath, const std::string& fragmentPath) {
  std::string vertex_shader = getByRelativePath(vertexPath);
  std::string fragment_shader = getByRelativePath(fragmentPath);

  return Shader(vertex_shader.c_str(), fragment_shader.c_str());
}