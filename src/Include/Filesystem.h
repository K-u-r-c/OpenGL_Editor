#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <filesystem>

#include "Shader.h"

namespace mfsys {

class filesystem {
 public:
  explicit filesystem(const std::filesystem::path &binary_path);

  ~filesystem();

  [[nodiscard]] std::filesystem::path get_binary_path() const;

  [[nodiscard]] std::string get(const std::string &path) const;

  [[nodiscard]] Shader create_shader(const std::string& vertex_path, const std::string& fragment_path) const;
  [[nodiscard]] unsigned int load_texture(const std::string &path) const;
  // TODO: Probably other create assets like texture, model, etc.

 private:
  std::filesystem::path binary_path_;
};

}  // namespace mfsys

#endif /* FILESYSTEM_H */
