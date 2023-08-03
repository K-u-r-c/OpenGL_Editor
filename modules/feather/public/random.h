#ifndef INC_3D_ENGINE_RANDOM_H
#define INC_3D_ENGINE_RANDOM_H

#include "cstdint"
#include "core.h"

namespace feather {

class random {
public:
  random();

  explicit random(uint32_t seed);

  /**
   * @brief Seed the random number generator with the given seed.
   * @param seed
   */
  void seed(uint32_t seed);

  /**
   * @brief Random bitstring from the stream.
   * @return uint32_t
   */
  uint32_t randomBits();

  /**
   * @brief Random real in range [0, 1]
   * @return real
   */
  real randomReal();

  /**
   * @brief Random real in range [0, scale]
   * @param scale
   * @return real
   */
  real randomReal(real scale);

  /**
   * @brief Random real in range [min, max]
   * @param min
   * @param max
   * @return real
   */
  real randomReal(real min, real max);

  /**
   * @brief Random int in range [0, max]
   * @param max
   * @return uint32_t
   */
  uint32_t randomInt(uint32_t max);

  /**
   * @brief Random real in range [-scale, scale]
   * @param scale
   * @return real
   */
  real randomBinomial(real scale);

  /**
   * @brief Random vector in range [-scale, scale]
   * @param scale
   * @return vector3
   */
  vector3 randomVector(real scale);

  /**
   * @brief Random vector where each component is binomially distributed in the range (-scale to scale) [mean = 0.0f].
   * @param scale
   * @return vector3
   */
  vector3 randomVector(const vector3& scale);

  /**
   * @brief Random vector in the cube defined by the given minimum and maximum vectors.
   * @param min
   * @param max
   * @return vector3
   */
  vector3 randomVector(const vector3& min, const vector3& max);

  /**
   * @brief Random vector where each component is binomially distributed in the range [-scale, scale] [mean = 0.0f], except the y coordinate which is zero.
   * @param scale
   * @return vector3
   */
  vector3 randomXZVector(real scale);

private:
  int p1, p2;
  uint32_t buffer[17];
};

} // feather

#endif //INC_3D_ENGINE_RANDOM_H
