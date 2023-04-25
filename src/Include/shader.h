#ifndef BA6A72A3_F6CB_41F9_89F1_9A3B467CDA87
#define BA6A72A3_F6CB_41F9_89F1_9A3B467CDA87

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>
#include <fstream>

class Shader {
 public:
  Shader(const char *vertexPath, const char *fragmentPath);
  Shader(const Shader &shader);
  ~Shader();

  void use(void);
  unsigned int getID(void);

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w);
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

 private:
  unsigned int ID;
};

#endif /* BA6A72A3_F6CB_41F9_89F1_9A3B467CDA87 */