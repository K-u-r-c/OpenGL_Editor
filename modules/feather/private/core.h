#ifndef INC_3D_ENGINE_CORE_H
#define INC_3D_ENGINE_CORE_H

#include "precision.h"

namespace feather {

class vector3 {
public:
  vector3();
  vector3(real x, real y, real z);

  void invert();

  void addScaledVector(const vector3 &vector, real scalar);

  [[nodiscard]] vector3 componentProduct(const vector3 &vector) const;

  void componentProductUpdate(const vector3 &vector);

  [[nodiscard]] real scalarProduct(const vector3 &vector) const;

  [[nodiscard]] vector3 vectorProduct(const vector3 &vector) const;

  [[nodiscard]] real magnitude() const;

  [[nodiscard]] real squareMagnitude() const;

  void normalize();

  void zero();

public:
  void operator*=(real scalar);

  vector3 operator*(real scalar) const;

  void operator+=(const vector3 &other);

  vector3 operator+(const vector3 &other) const;

  void operator-=(const vector3 &other);

  vector3 operator-(const vector3 &other) const;

  /**
   * @brief Dot product
   * @param vector3
   * @return Scalar
   */
  real operator*(const vector3 &other) const;

  /**
   * @brief Cross product
   * @param vector3
   * @return void
   */
  void operator%=(const vector3 &other);

  /**
   * @brief Cross product
   * @param vector3
   * @return Vector
   */
  vector3 operator%(const vector3 &other) const;

  bool operator==(const vector3 &other) const;

public:
  real x;
  real y;
  real z;

private:
  real w_; // Padding to 16 bytes if single precision and 32 bytes if double precision
};

inline void vector3::operator*=(const real scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
}

inline vector3 vector3::operator*(const real scalar) const {
  return {x * scalar, y * scalar, z * scalar};
}

inline void vector3::operator+=(const vector3 &other) {
  x += other.x;
  y += other.y;
  z += other.z;
}

inline vector3 vector3::operator+(const vector3 &other) const {
  return {x + other.x, y + other.y, z + other.z};
}

inline void vector3::operator-=(const vector3 &other) {
  x -= other.x;
  y -= other.y;
  z -= other.z;
}

inline vector3 vector3::operator-(const vector3 &other) const {
  return {x - other.x, y - other.y, z - other.z};
}

inline real vector3::operator*(const vector3 &other) const {
  return x * other.x + y * other.y + z * other.z;
}

inline void vector3::operator%=(const vector3 &other) {
  *this = *this % other;
}

inline vector3 vector3::operator%(const vector3 &other) const {
  return {y * other.z - z * other.y,
          z * other.x - x * other.z,
          x * other.y - y * other.x};
}

inline bool vector3::operator==(const vector3 &other) const {
  return x == other.x && y == other.y && z == other.z;
}

} // namespace feather

#endif //INC_3D_ENGINE_CORE_H
