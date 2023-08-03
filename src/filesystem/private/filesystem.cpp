#include "../public/filesystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <sstream>

mfsys::filesystem::filesystem(const std::filesystem::path &binary_path) {
  if (!std::filesystem::exists(binary_path)) throw std::runtime_error("Binary path does not exist");

  if (binary_path.string().find(".app") != std::string::npos) {
    binary_path_ = binary_path.parent_path().parent_path() / "Resources";
  } else {
    binary_path_ = binary_path.parent_path();
  }

#ifdef DEBUG
  std::cout << "Binary path: " << binary_path_ << std::endl;
#endif
}

mfsys::filesystem::~filesystem() = default;

std::filesystem::path mfsys::filesystem::get_binary_path() const { return binary_path_; }

std::string mfsys::filesystem::get(const std::string &path) const {
  std::string result = binary_path_.string();

  constexpr char separator = std::filesystem::path::preferred_separator;
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

shader mfsys::filesystem::create_shader(const std::string &vertex_path, const std::string &fragment_path) const {
  const std::string vertexShader = get(vertex_path);
  const std::string fragment_shader = get(fragment_path);

  return {vertexShader, fragment_shader};
}

uint32_t mfsys::filesystem::load_texture(const std::string &path) const {
  uint32_t texture_id;
  glGenTextures(1, &texture_id);

  const std::string texture = get(path);

  int32_t width, height, nr_components;
  if (unsigned char *data = stbi_load(texture.c_str(), &width, &height, &nr_components, 0)) {
    GLint format = 0;
    if (nr_components == 1)
      format = GL_RED;
    else if (nr_components == 3)
      format = GL_RGB;
    else if (nr_components == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture_id);
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

  return texture_id;
}
