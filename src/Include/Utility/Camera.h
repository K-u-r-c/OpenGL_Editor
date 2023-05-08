#ifndef CAMERA_H
#define CAMERA_H

#include <cassert>
#include <algorithm>

#include "GLM/glm/gtx/euler_angles.hpp"

class camera_positioner_interface {
 public:
  virtual ~camera_positioner_interface() = default;

  [[nodiscard]] virtual glm::mat4 get_view_matrix() const = 0;
  [[nodiscard]] virtual glm::vec3 get_position() const = 0;
  [[nodiscard]] virtual float get_fov() const = 0;
  [[nodiscard]] virtual float get_z_near() const = 0;
  [[nodiscard]] virtual float get_z_far() const = 0;
};

class camera final {
 public:
  explicit camera(const camera_positioner_interface& positioner) : positioner_(&positioner) {}

  camera(const camera&) = default;
  camera& operator=(const camera&) = default;

  [[nodiscard]] glm::mat4 get_view_matrix() const { return positioner_->get_view_matrix(); }
  [[nodiscard]] glm::vec3 get_position() const { return positioner_->get_position(); }
  [[nodiscard]] float get_fov() const { return positioner_->get_fov(); }
  [[nodiscard]] float get_z_near() const { return positioner_->get_z_near(); }
  [[nodiscard]] float get_z_far() const { return positioner_->get_z_far(); }

 private:
  const camera_positioner_interface* positioner_;
};

class camera_positioner_first_person final : public camera_positioner_interface {
 public:
  camera_positioner_first_person() = default;
  camera_positioner_first_person(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
    : camera_position_(pos), camera_orientation_(glm::lookAt(pos, target, up)), up_(up) {}

  void update(double delta_seconds, const glm::vec2& mouse_pos, bool mousePressed) {
    if (mousePressed) {
      const glm::vec2 delta = mouse_pos - mouse_pos_;
      glm::quat pitch = glm::angleAxis(delta.y * mouse_speed, glm::vec3(1.0f, 0.0f, 0.0f));
      glm::quat yaw = glm::angleAxis(delta.x * mouse_speed, glm::vec3(0.0f, 1.0f, 0.0f));
      camera_orientation_ = pitch * camera_orientation_ * yaw;
    }

    mouse_pos_ = mouse_pos;

    const glm::mat4 v = glm::mat4_cast(camera_orientation_);

    const glm::vec3 forward = -glm::vec3(v[0][2], v[1][2], v[2][2]);
    const glm::vec3 right = glm::vec3(v[0][0], v[1][0], v[2][0]);
    const glm::vec3 up = glm::cross(right, forward);

    glm::vec3 accel(0.0f);

    if(movement.forward) accel += forward;
    if(movement.backward) accel -= forward;

    if(movement.left) accel -= right;
    if(movement.right) accel += right;

    if(movement.up) accel += up;
    if(movement.down) accel -= up;

    if (movement.fast_speed) accel *= fast_coef;

    if(accel == glm::vec3(0)) {
      move_speed_ -= move_speed_ * std::min((1.0f / damping) * static_cast<float>(delta_seconds), 1.0f);
    } else {
      move_speed_ += accel * acceleration * static_cast<float>(delta_seconds);
      const float speed = movement.fast_speed ? max_speed * fast_coef : max_speed;
      if(glm::length(move_speed_) > speed) move_speed_ = glm::normalize(move_speed_) * max_speed;
    }

    camera_position_ += move_speed_ * static_cast<float>(delta_seconds);
  }

  [[nodiscard]] glm::mat4 get_view_matrix() const override {
    const glm::mat4 t = glm::translate(glm::mat4(1.0f), -camera_position_);
    const glm::mat4 r = glm::mat4_cast(camera_orientation_);

    return r * t;
  }

  [[nodiscard]] glm::vec3 get_position() const override { return camera_position_; }
  void set_position(const glm::vec3& pos) { camera_position_ = pos; }

  void reset_mouse_position(const glm::vec2& p) { mouse_pos_ = p; }

  [[nodiscard]] float get_fov() const override { return fov_; }
  void set_fov(const float fov) { fov_ = fov; }

  [[nodiscard]] float get_z_near() const override { return z_near_; }
  void set_z_near(const float z_near);

  [[nodiscard]] float get_z_far() const override { return z_far_; }
  void set_z_far(const float z_far) { z_far_ = z_far; }

  struct movement {
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool fast_speed = false;
  } movement;

  float mouse_speed = 4.0f;
  float acceleration = 150.0f;
  float damping = 0.2f;
  float max_speed = 10.0f;
  float fast_coef = 10.0f;

 private:
  glm::vec2 mouse_pos_ = glm::vec2(0);
  glm::vec3 camera_position_ = glm::vec3(0.0f, 10.0f, 10.0f);
  glm::quat camera_orientation_ = glm::quat(glm::vec3(0));
  glm::vec3 move_speed_ = glm::vec3(0);
  glm::vec3 up_ = glm::vec3(0.0f, 0.0f, 1.0f);
  float fov_ = 45.0f;
  float z_near_ = 0.1f;
  float z_far_ = 1000.0f;
};

inline void camera_positioner_first_person::set_z_near(const float z_near)
{ z_near_ = z_near; }

#endif //CAMERA_H
