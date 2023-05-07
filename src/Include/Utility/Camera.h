#ifndef CAMERA_H
#define CAMERA_H

#include <cassert>
#include <algorithm>

#include "GLM/glm/gtx/euler_angles.hpp"

class CameraPositionerInterface {
 public:
  virtual ~CameraPositionerInterface() = default;

  [[nodiscard]] virtual glm::mat4 getViewMatrix() const = 0;
  [[nodiscard]] virtual glm::vec3 getPosition() const = 0;
  [[nodiscard]] virtual float getFOV() const = 0;
  [[nodiscard]] virtual float getZNear() const = 0;
  [[nodiscard]] virtual float getZFar() const = 0;
};

class Camera final {
 public:
  explicit Camera(CameraPositionerInterface& positioner) : positioner_(&positioner) {}

  Camera(const Camera&) = default;
  Camera& operator=(const Camera&) = default;

  [[nodiscard]] glm::mat4 getViewMatrix() const { return positioner_->getViewMatrix(); }
  [[nodiscard]] glm::vec3 getPosition() const { return positioner_->getPosition(); }
  [[nodiscard]] float getFOV() const { return positioner_->getFOV(); }
  [[nodiscard]] float getZNear() const { return positioner_->getZNear(); }
  [[nodiscard]] float getZFar() const { return positioner_->getZFar(); }

 private:
  const CameraPositionerInterface* positioner_;
};

class CameraPositioner_FirstPerson final : public CameraPositionerInterface {
 public:
  CameraPositioner_FirstPerson() = default;
  CameraPositioner_FirstPerson(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
    : _cameraPosition(pos), _cameraOrientation(glm::lookAt(pos, target, up)), _up(up) {}

  void update(double deltaSeconds, const glm::vec2& mousePos, bool mousePressed) {
    if (mousePressed) {
      const glm::vec2 delta = mousePos - _mousePos;
      glm::quat pitch = glm::angleAxis(delta.y * mouseSpeed, glm::vec3(1.0f, 0.0f, 0.0f));
      glm::quat yaw = glm::angleAxis(delta.x * mouseSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
      _cameraOrientation = pitch * _cameraOrientation * yaw;
    }

    _mousePos = mousePos;

    const glm::mat4 v = glm::mat4_cast(_cameraOrientation);

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

    if (movement.fastSpeed) accel *= fastCoef;

    if(accel == glm::vec3(0)) {
      _moveSpeed -= _moveSpeed * std::min((1.0f / damping) * static_cast<float>(deltaSeconds), 1.0f);
    } else {
      _moveSpeed += accel * acceleration * static_cast<float>(deltaSeconds);
      const float speed = movement.fastSpeed ? maxSpeed * fastCoef : maxSpeed;
      if(glm::length(_moveSpeed) > speed) _moveSpeed = glm::normalize(_moveSpeed) * maxSpeed;
    }

    _cameraPosition += _moveSpeed * static_cast<float>(deltaSeconds);
  }

  [[nodiscard]] glm::mat4 getViewMatrix() const override {
    const glm::mat4 t = glm::translate(glm::mat4(1.0f), -_cameraPosition);
    const glm::mat4 r = glm::mat4_cast(_cameraOrientation);

    return r * t;
  }

  [[nodiscard]] glm::vec3 getPosition() const override { return _cameraPosition; }
  void setPosition(const glm::vec3& pos) { _cameraPosition = pos; }

  void resetMousePosition(const glm::vec2& p) { _mousePos = p; }

  [[nodiscard]] float getFOV() const override { return _fov; }
  void setFOV(const float fov) { _fov = fov; }

  [[nodiscard]] float getZNear() const override { return _zNear; }
  void setZNear(const float zNear) { _zNear = zNear; }

  [[nodiscard]] float getZFar() const override { return _zFar; }
  void setZFar(const float zFar) { _zFar = zFar; }

  struct Movement {
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool fastSpeed = false;
  } movement;

  float mouseSpeed = 4.0f;
  float acceleration = 150.0f;
  float damping = 0.2f;
  float maxSpeed = 10.0f;
  float fastCoef = 10.0f;

 private:
  glm::vec2 _mousePos = glm::vec2(0);
  glm::vec3 _cameraPosition = glm::vec3(0.0f, 10.0f, 10.0f);
  glm::quat _cameraOrientation = glm::quat(glm::vec3(0));
  glm::vec3 _moveSpeed = glm::vec3(0);
  glm::vec3 _up = glm::vec3(0.0f, 0.0f, 1.0f);
  float _fov = 45.0f;
  float _zNear = 0.1f;
  float _zFar = 1000.0f;
};

#endif //CAMERA_H
