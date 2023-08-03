#include "random.h"
#include "ctime"
#include "climits"

/**
 * Takes a 32-bit integer and rotates its binary representation to the left by a specified number of bits.
 * The bits that "fall off" from the left side are brought back to the right side.
 * The rotation is circular, meaning it wraps around.
 */
static inline uint32_t rotl32(uint32_t n, uint32_t c) {
  const uint32_t mask = (CHAR_BIT * sizeof(n) - 1);

  c &= mask;
  return (n << c) | (n >> ((-c) & mask));
}


namespace feather {


random::random() {
  seed(0);
}

random::random(uint32_t seed) {
  random::seed(seed);
}

void random::seed(uint32_t seed) {
  if (seed == 0) {
    seed = static_cast<uint32_t>(clock());
  }

  for (uint32_t& i : buffer) {
    /** large prime constant chosen to provide good randomness properties. */
    const uint32_t LCGrandomNumber = 2891336453;

    seed = seed * LCGrandomNumber + 1;
    i = seed;
  }

  p1 = 0;
  p2 = 10;
}

uint32_t random::randomBits() {
  uint32_t result;

  result = buffer[p1] = rotl32(buffer[p2], 13) + rotl32(buffer[p1], 9);

  if (--p1 < 0) p1 = 16;
  if (--p2 < 0) p2 = 16;

  return result;
}

#ifdef SINGLE_PRECISION


real random::randomReal() {
  unsigned bits = randomBits();

  /** Set up a reinterpret structure for manipulation */
  union {
    real value;
    unsigned word;
  } convert{};

  /**
   * Now assign the bits to the word. This works by fixing the ieee
   * sign and exponent bits (so that the size of the result is 1-2)
   * and using the bits to create the fraction part of the float.
   */
  convert.word = (bits >> 9) | 0x3f800000;

  return convert.value - 1.0f;
}


#else


real random::randomReal() {
  uint32_t bits = randomBits();

  /** Set up a reinterpret structure for manipulation */
  union {
    real value;
    uint32_t words[2];
  } convert{};

  /**
   * Now assign the bits to the words. This works by fixing the ieee
   * sign and exponent bits (so that the size of the result is 1-2)
   * and using the bits to create the fraction part of the float. Note
   * that bits are used more than once in this process.
   */
  convert.words[0] = bits << 20;                 // Fill in the top 16 bits
  convert.words[1] = (bits >> 12) | 0x3FF00000;  // And the bottom 20

  // And return the value
  return convert.value - 1.0;
}


#endif

real random::randomReal(real scale) {
  return randomReal() * scale;
}

real random::randomReal(real min, real max) {
  return randomReal() * (max - min) + min;
}

uint32_t random::randomInt(uint32_t max) {
  return randomBits() % max;
}

real random::randomBinomial(real scale) {
  return (randomReal() - randomReal()) * scale;
}

vector3 random::randomVector(real scale) {
  return {randomBinomial(scale), randomBinomial(scale), randomBinomial(scale)};
}

vector3 random::randomVector(const vector3 &scale) {
  return {randomBinomial(scale.x), randomBinomial(scale.y), randomBinomial(scale.z)};
}

vector3 random::randomVector(const vector3 &min, const vector3 &max) {
  return {randomReal(min.x, max.x), randomReal(min.y, max.y), randomReal(min.z, max.z)};
}

vector3 random::randomXZVector(real scale) {
  return {randomBinomial(scale), 0, randomBinomial(scale)};
}


} // feather