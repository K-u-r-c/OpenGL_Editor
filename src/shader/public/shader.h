#ifndef SHADER_H
#define SHADER_H

#include "GLAD/glad/glad.h"

#include "GLM/glm/glm.hpp"

#include <string>
#include <fstream>

class shader {
 public:
  shader(const std::string &vertexPath, const std::string &fragmentPath);

  shader(const shader &shader);

  ~shader();

  void use() const;

  [[nodiscard]] unsigned int getID() const;

  void setBool(const std::string &name, bool value) const;

  void setInt(const std::string &name, int32_t value) const;

  void setFloat(const std::string &name, float_t value) const;

  void setVec2(const std::string &name, const glm::vec2 &value) const;

  void setVec2(const std::string &name, float_t x, float_t y) const;

  void setVec3(const std::string &name, const glm::vec3 &value) const;

  void setVec3(const std::string &name, float_t x, float_t y, float_t z) const;

  void setVec4(const std::string &name, const glm::vec4 &value) const;

  void setVec4(const std::string &name, float_t x, float_t y, float_t z, float_t w) const;

  void setMat2(const std::string &name, const glm::mat2 &mat) const;

  void setMat3(const std::string &name, const glm::mat3 &mat) const;

  void setMat4(const std::string &name, const glm::mat4 &mat) const;

 private:
  unsigned int ID;
};

#endif /* SHADER_H */
