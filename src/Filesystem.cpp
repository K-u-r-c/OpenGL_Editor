#include "Filesystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <sstream>

mfsys::Filesystem::Filesystem(const std::filesystem::path &path) {
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

mfsys::Filesystem::~Filesystem() = default;

std::filesystem::path mfsys::Filesystem::getBinaryPath() const { return binaryPath; }

std::string mfsys::Filesystem::get(const std::string &path) const {
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

Shader mfsys::Filesystem::createShader(const std::string &vertexPath, const std::string &fragmentPath) const {
  std::string vertex_shader = get(vertexPath);
  std::string fragment_shader = get(fragmentPath);

  return {vertex_shader.c_str(), fragment_shader.c_str()};
}

unsigned int mfsys::Filesystem::loadTexture(const std::string &path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
