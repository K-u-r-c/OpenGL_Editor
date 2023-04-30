#ifndef FRAMES_PER_SECOND_COUNTER_H
#define FRAMES_PER_SECOND_COUNTER_H

#include <cassert>
#include <iostream>

class FramesPerSecondCounter {
 public:
  explicit FramesPerSecondCounter(float avgIntervalSec = 0.5f) : _avgIntervalSec(avgIntervalSec) {
    assert(avgIntervalSec > 0.0f);
  }

  bool tick(float deltaSeconds, bool frameRendered = true) {
    if (frameRendered) _numFrames++;
    _accumulatedTime += deltaSeconds;

    if(_accumulatedTime < _avgIntervalSec) return false;

    _currentFPS = static_cast<double>(_numFrames) / _accumulatedTime;
    std::cout << "FPS: " << _currentFPS << std::endl;

    _numFrames = 0;
    _accumulatedTime = 0.0;

    return true;
  }

  [[nodiscard]] inline double getFPS() const { return _currentFPS; }

 private:
  const float _avgIntervalSec = 0.5f;
  unsigned int _numFrames = 0;
  double _accumulatedTime = 0.0;
  double _currentFPS = 0.0;
};

#endif // FRAMES_PER_SECOND_COUNTER_H
