#include "time.hpp"

#include <chrono>

#include <iostream>

namespace timer {
std::chrono::time_point<std::chrono::high_resolution_clock> timers_[8] = {
    std::chrono::time_point<std::chrono::high_resolution_clock>(),
    std::chrono::time_point<std::chrono::high_resolution_clock>(),
    std::chrono::time_point<std::chrono::high_resolution_clock>(),
    std::chrono::time_point<std::chrono::high_resolution_clock>(),
    std::chrono::time_point<std::chrono::high_resolution_clock>(),
    std::chrono::time_point<std::chrono::high_resolution_clock>(),
    std::chrono::time_point<std::chrono::high_resolution_clock>(),
    std::chrono::time_point<std::chrono::high_resolution_clock>()};
}  // namespace timer

std::chrono::time_point<std::chrono::high_resolution_clock> timer::start() {
  return std::chrono::high_resolution_clock::now();
}
double timer::stop(
    const std::chrono::time_point<std::chrono::high_resolution_clock>&
        start_time_point) {
  return std::chrono::duration<double>(
             std::chrono::high_resolution_clock::now() - start_time_point)
      .count();
}

void timer::start(const short unsigned& id) {
  timers_[id] = std::chrono::high_resolution_clock::now();
}

double timer::stop(const short unsigned& id) {
  return std::chrono::duration<double>(
             std::chrono::high_resolution_clock::now() - timers_[id])
      .count();
}
