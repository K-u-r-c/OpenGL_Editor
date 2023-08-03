#include "../public/camera.h"

// CAMERA

camera::camera(const camera_positioner_interface& positioner) : positioner_(&positioner) {}

camera::camera(const camera&) = default;

camera& camera::operator=(const camera&) = default;

glm::mat4 camera::get_view_matrix() const { return positioner_->get_view_matrix(); }

glm::vec3 camera::get_position() const { return positioner_->get_position(); }

float_t camera::get_fov() const { return positioner_->get_fov(); }

float_t camera::get_z_near() const { return positioner_->get_z_near(); }

float_t camera::get_z_far() const { return positioner_->get_z_far(); }

// CAMERA POSITIONER

camera_positioner_first_person::camera_positioner_first_person() = default;

camera_positioner_first_person::camera_positioner_first_person(
      const glm::vec3& pos,
      const glm::vec3& target,
      const glm::vec3& up
    ) : camera_position_(pos), camera_orientation_(glm::lookAt(pos, target, up)), up_(up) {}

void camera_positioner_first_person::update(double_t delta_seconds, const glm::vec2& mouse_pos, bool mousePressed) {
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
    move_speed_ += accel * acceleration * static_cast<float_t>(delta_seconds);
    const float speed = movement.fast_speed ? max_speed * fast_coef : max_speed;
    if(glm::length(move_speed_) > speed) move_speed_ = glm::normalize(move_speed_) * max_speed;
  }

  camera_position_ += move_speed_ * static_cast<float_t>(delta_seconds);
}

glm::mat4 camera_positioner_first_person::get_view_matrix() const {
  const glm::mat4 t = glm::translate(glm::mat4(1.0f), -camera_position_);
  const glm::mat4 r = glm::mat4_cast(camera_orientation_);

  return r * t;
}

glm::vec3 camera_positioner_first_person::get_position() const { return camera_position_; }

void camera_positioner_first_person::set_position(const glm::vec3& pos) { camera_position_ = pos; }

void camera_positioner_first_person::reset_mouse_position(const glm::vec2& p) { mouse_pos_ = p; }

float_t camera_positioner_first_person::get_fov() const { return fov_; }

void camera_positioner_first_person::set_fov(const float_t fov) { fov_ = fov; }

float_t camera_positioner_first_person::get_z_near() const { return z_near_; }

void camera_positioner_first_person::set_z_near(const float_t z_near) { z_near_ = z_near; }

float_t camera_positioner_first_person::get_z_far() const { return z_far_; }

void camera_positioner_first_person::set_z_far(const float_t z_far) { z_far_ = z_far; }
