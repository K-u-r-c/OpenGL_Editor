#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <filesystem>

#include "Shader.h"

namespace mfsys {

class Filesystem {
 public:
  explicit Filesystem(const std::filesystem::path &binaryPath);

  ~Filesystem();

  [[nodiscard]] std::filesystem::path getBinaryPath() const;

  [[nodiscard]] std::string get(const std::string &path) const;

  [[nodiscard]] Shader createShader(const std::string &vertexPath, const std::string &fragmentPath) const;
  // TODO: Probably other create assets like texture, model, etc.

 private:
  std::filesystem::path binaryPath;
};

}  // namespace mfsys

#endif /* FILESYSTEM_H */
