#include "core.h"

using namespace feather;

vector3::vector3()
  : x(0), y(0), z(0) {

  w_ = 0;
}

vector3::vector3(feather::real x, feather::real y, feather::real z)
  : x(x), y(y), z(z) {

  w_ = 0;
}

void vector3::invert() {
  x = -x;
  y = -y;
  z = -z;
}

void vector3::addScaledVector(const vector3 &vector, real scalar) {
  x += vector.x * scalar;
  y += vector.y * scalar;
  z += vector.z * scalar;
}

vector3 vector3::componentProduct(const vector3 &vector) const {
  return {x * vector.x, y * vector.y, z * vector.z};
}

void vector3::componentProductUpdate(const vector3 &vector) {
  x *= vector.x;
  y *= vector.y;
  z *= vector.z;
}

real vector3::scalarProduct(const vector3 &vector) const {
  return x * vector.x + y * vector.y + z * vector.z;
}

vector3 vector3::vectorProduct(const vector3 &vector) const {
  return {y * vector.z - z * vector.y,
          z * vector.x - x * vector.z,
          x * vector.y - y * vector.x};
}

real vector3::magnitude() const {
  return real_sqrt(x * x + y * y + z * z);
}

real vector3::squareMagnitude() const {
  return x * x + y * y + z * z;
}

void vector3::normalize() {
  real l = magnitude();
  if (l > 0) {
    (*this) *= ((real) 1) / l;
  }
}

void vector3::zero() {
  x = y = z = 0;
}