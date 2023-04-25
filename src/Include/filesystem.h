#ifndef F7CC37F7_B62E_4F81_9776_D32F7236FCF3
#define F7CC37F7_B62E_4F81_9776_D32F7236FCF3

#include <string>
#include <filesystem>

#include "shader.h"

namespace MyFilesystem {

class File {
 public:
  File(const std::filesystem::path& binaryPath);
  ~File();

  std::filesystem::path getPath() const;

  Shader createShader(const std::string& vertexPath, const std::string& fragmentPath);
  // TODO: Probably other create assets like texture, model, etc.

 private:
  std::filesystem::path binaryPath;
};

}  // namespace MyFilesystem

#endif /* F7CC37F7_B62E_4F81_9776_D32F7236FCF3 */
