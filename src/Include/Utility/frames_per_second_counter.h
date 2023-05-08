#ifndef FRAMES_PER_SECOND_COUNTER_H
#define FRAMES_PER_SECOND_COUNTER_H

#include <cassert>
#include <iostream>

class frames_per_second_counter {
 public:
  explicit frames_per_second_counter(const double avg_interval_sec = 0.5) : avg_interval_sec_(avg_interval_sec) {
    assert(avg_interval_sec > 0.0);
  }

  bool tick(const double delta_seconds, const bool frame_rendered = true) {
    if (frame_rendered) num_frames_++;
    accumulated_time_ += delta_seconds;

    if(accumulated_time_ < avg_interval_sec_) return false;

    current_fps_ = static_cast<double>(num_frames_) / accumulated_time_;
    std::cout << "FPS: " << current_fps_ << std::endl;

    num_frames_ = 0;
    accumulated_time_ = 0.0;

    return true;
  }

  [[nodiscard]] inline double get_fps() const { return current_fps_; }

 private:
  const double avg_interval_sec_ = 0.5;
  unsigned int num_frames_ = 0;
  double accumulated_time_ = 0.0;
  double current_fps_ = 0.0;
};

#endif // FRAMES_PER_SECOND_COUNTER_H
