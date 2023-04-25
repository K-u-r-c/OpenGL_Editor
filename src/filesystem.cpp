#include "filesystem.h"
#include <iostream>

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

std::filesystem::path MyFilesystem::File::getPath() const { return binaryPath; }

Shader MyFilesystem::File::createShader(const std::string& vertexPath, const std::string& fragmentPath) {
  std::filesystem::path vertexShaderPath = binaryPath / vertexPath;
  std::filesystem::path fragmentShaderPath = binaryPath / fragmentPath;

  return Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
}