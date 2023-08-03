#ifndef CAMERA_H
#define CAMERA_H

#include "algorithm"

#include "GLM/glm/gtx/euler_angles.hpp"

class camera_positioner_interface {
 public:
  virtual ~camera_positioner_interface() = default;

  [[nodiscard]] virtual glm::mat4 get_view_matrix() const = 0;
  [[nodiscard]] virtual glm::vec3 get_position() const = 0;
  [[nodiscard]] virtual float_t get_fov() const = 0;
  [[nodiscard]] virtual float_t get_z_near() const = 0;
  [[nodiscard]] virtual float_t get_z_far() const = 0;
};

class camera final {
 public:
  explicit camera(const camera_positioner_interface& positioner);
  camera(const camera&);
  camera& operator=(const camera&);

  [[nodiscard]] glm::mat4 get_view_matrix() const;
  [[nodiscard]] glm::vec3 get_position() const;
  [[nodiscard]] float_t get_fov() const;
  [[nodiscard]] float_t get_z_near() const;
  [[nodiscard]] float_t get_z_far() const;

 private:
  const camera_positioner_interface* positioner_;
};

class camera_positioner_first_person final : public camera_positioner_interface {
 public:
  camera_positioner_first_person();
  camera_positioner_first_person(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

  void update(double_t delta_seconds, const glm::vec2& mouse_pos, bool mousePressed);

  [[nodiscard]] glm::mat4 get_view_matrix() const override;

  [[nodiscard]] glm::vec3 get_position() const override;
  void set_position(const glm::vec3& pos);

  void reset_mouse_position(const glm::vec2& p);

  [[nodiscard]] float_t get_fov() const override;
  void set_fov(const float_t fov);

  [[nodiscard]] float_t get_z_near() const override;
  void set_z_near(const float_t z_near);

  [[nodiscard]] float_t get_z_far() const override;
  void set_z_far(const float_t z_far);

public:
  struct movement {
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool fast_speed = false;
  } movement;

public:
  float_t mouse_speed = 4.0f;
  float_t acceleration = 150.0f;
  float_t damping = 0.2f;
  float_t max_speed = 10.0f;
  float_t fast_coef = 10.0f;

 private:
  glm::vec2 mouse_pos_ = glm::vec2(0);
  glm::vec3 camera_position_ = glm::vec3(0.0f, 10.0f, 10.0f);
  glm::quat camera_orientation_ = glm::quat(glm::vec3(0));
  glm::vec3 move_speed_ = glm::vec3(0);
  glm::vec3 up_ = glm::vec3(0.0f, 0.0f, 1.0f);
  float_t fov_ = 45.0f;
  float_t z_near_ = 0.1f;
  float_t z_far_ = 1000.0f;
};

#endif //CAMERA_H
