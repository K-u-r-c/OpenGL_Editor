#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <filesystem>

#include "shader.h"

namespace mfsys {

class filesystem {
 public:
  explicit filesystem(const std::filesystem::path &binaryPath);

  ~filesystem();

  [[nodiscard]] std::filesystem::path getBinaryPath() const;

  [[nodiscard]] std::string get(const std::string &path) const;

  [[nodiscard]] shader createShader(const std::string &vertexPath, const std::string &fragmentPath) const;
  // TODO: Probably other create assets like texture, model, etc.

 private:
  std::filesystem::path binaryPath;
};

}  // namespace mfsys

#endif /* FILESYSTEM_H */
